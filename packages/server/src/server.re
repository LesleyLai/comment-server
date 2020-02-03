open Express;

open Common;
open Comment;

// an imperative data structure that store all the comments
module type Database = {
  type t;

  // Create an empty comment Database
  let create: t;

  // Filter comments by slug
  let filterBySlug: (t, string) => Js.Dict.t(withChildren);

  // Adds a comment into the database
  let add: (t, withParent) => unit;
};

module MockDatabase: Database = {
  module IntMap =
    Map.Make({
      type t = int;
      let compare: (int, int) => int = compare;
    });

  type t = ref(IntMap.t(withParent));

  let currentId = ref(0);

  let create = ref(IntMap.empty);

  let filterBySlug = (db: t, slug) => {
    let dict = Js.Dict.empty();
    let flatDict = Js.Dict.empty();

    IntMap.iter(
      (id, commentWithParent: withParent) => {
        let idString = id |> string_of_int;
        if (commentWithParent.comment->Comment.slug == slug) {
          let withChildren =
            Comment.createWithChildren(
              ~comment=commentWithParent.comment,
              ~children=Js.Dict.empty(),
            );
          Js.Dict.set(flatDict, idString, withChildren);

          switch (commentWithParent.parent_id) {
          | None => Js.Dict.set(dict, idString, withChildren)
          | Some(parent_id) =>
            let parent =
              Js.Dict.unsafeGet(flatDict, parent_id |> string_of_int);
            Js.Dict.set(parent.children, idString, withChildren);
          };
        };
      },
      db^,
    );

    dict;
  };

  let add = (db, withParent) => {
    db := db^ |> IntMap.add(currentId^, withParent);
    currentId := currentId^ + 1;
  };
};

let getDictString = (dict, key) =>
  switch (Js.Dict.get(dict, key)) {
  | Some(json) => Js.Json.decodeString(json)
  | _ => None
  };

// Express Server

[@bs.module] external cors: 'a => Express.Middleware.t = "cors";

module MakeServer = (DB: Database) => {
  let create = {
    let database = DB.create;

    let comment1 =
      Comment.create(
        ~commenter=Guest({name: "bob", url: None}),
        ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
        ~slug="lesleylai.info",
        ~text="Test the functionality of the comments",
      );

    let comment2 =
      Comment.create(
        ~commenter=Guest({name: "bob2", url: Some("bob.com")}),
        ~date=Js.Date.makeWithYMD(~year=2020., ~month=2., ~date=1., ()),
        ~slug="lesleylai.info",
        ~text="Reply",
      );

    DB.add(
      database,
      Comment.createWithParent(~comment=comment1, ~parent_id=None),
    );

    DB.add(
      database,
      Comment.createWithParent(~comment=comment2, ~parent_id=Some(0)),
    );

    let app = express();

    // TODO: only enable cors conditionally
    app->App.use(cors());
    app->App.use(Middleware.json());
    app->App.use(Middleware.urlencoded());

    // Gets comments
    App.get(app, ~path="/comments/:slug") @@
    Middleware.from((next, req) =>
      switch (getDictString(Request.params(req), "slug")) {
      | Some(slug) =>
        database->DB.filterBySlug(slug)
        |> Comment.Encode.commentWithChildrenDict
        |> Response.sendJson
      | _ => next(Next.route)
      }
    );

    // Creates a comment
    App.post(app, ~path="/comment") @@
    Middleware.from((next, req) =>
      switch (Request.bodyJSON(req)) {
      | Some(json) =>
        json |> Comment.Decode.commentWithParent |> database->DB.add;
        Response.sendStatus(Ok);
      | None => next(Next.route)
      }
    );

    app;
  };
};
