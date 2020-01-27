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
  module IntMap = Map.Make({
    type t = int;
    let compare: (int, int) => (int) = compare;
  });

  type t = ref(IntMap.t(withParent));

  let currentId = ref(0);

  let create = ref(IntMap.empty);

  let filterBySlug = (db: t, slug) =>
  {
    let dict = Js.Dict.empty();
    let flatDict = Js.Dict.empty();

    IntMap.iter(
      (id, commentWithParent: withParent) => {
          let idString = id |> string_of_int;
          if ((commentWithParent.comment)->Comment.slug == slug) {
            switch (commentWithParent.parent_id) {
              | None => {
                let withChildren = Comment.createWithChildren(
                  ~comment= commentWithParent.comment,
                  ~children= Js.Dict.empty(),
                );
                Js.Dict.set(flatDict, idString, withChildren);
                Js.Dict.set(dict, idString, withChildren);
              }
              | Some(parent_id) => {
                let withChildren = Comment.createWithChildren(
                  ~comment= commentWithParent.comment,
                  ~children= Js.Dict.empty(),
                );
                Js.Dict.set(flatDict, idString, withChildren);
                let parent = Js.Dict.unsafeGet(flatDict, parent_id |> string_of_int);
                Js.Dict.set(parent.children, idString, withChildren);
              }
            }
          }
        },
      db^
    );

    dict
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

    let comment1 = Comment.create(
        ~commenter=Guest({name: "bob", url: None}),
        ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
        ~slug="lesleylai.info",
        ~text="Test the functionality of the comments",
      );

    let comment2 = Comment.create(
        ~commenter=Guest({name: "bob2", url: Some("bob.com")}),
        ~date=Js.Date.makeWithYMD(~year=2020., ~month=2., ~date=1., ()),
        ~slug="lesleylai.info",
        ~text="Reply",
      );

    DB.add(
      database,
      Comment.createWithParent(~comment= comment1, ~parent_id= None),
    );

    DB.add(
      database,
      Comment.createWithParent(~comment= comment2, ~parent_id= Some(0)),
    );

    let app = express();

    // TODO: only enable cors conditionally
    App.use(app, cors());

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

    let port =
      switch (Js.Nullable.toOption(Env.port)) {
      | Some(port) => int_of_string(port)
      | None =>
        Js.log @@ "Cannot find PORT envirnment variable: default to 3000";
        3000;
      };

    App.listen(
      app,
      ~port,
      ~onListen=
        e =>
          switch (e) {
          | exception (Js.Exn.Error(e)) =>
            Js.log(e);
            Node.Process.exit(1);
          | _ => Printf.printf("Listening at localhost:%d", port)
          },
      (),
    );
  };
};

module Server = MakeServer(MockDatabase);

let server = Server.create;
