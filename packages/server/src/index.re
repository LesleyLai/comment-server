open Express;

// an imperative data structure that store all the comments
module type Database {
  type t;

  // Create an empty comment Database
  let create: t;

  // Filter comments by slug
  let filterBySlug: t => string => list(Comment.t);

  // Adds a comment into the database
  let add: t => Comment.t => unit;
};

module MockDatabase : Database {
  type t = Hashtbl.t(int, Comment.t);

  let create = Hashtbl.create(10);

  let filterBySlug = (db, slug) =>
    Hashtbl.fold(((_, comment) => (acc) => {
    if (comment->Comment.slug == slug) {
      [comment, ...acc]
    } else {
      acc
    }
  }), db, []);

  let add = (db, comment) =>
    Hashtbl.add(db, comment->Comment.id, comment);
};


let getDictString = (dict, key) =>
  switch (Js.Dict.get(dict, key)) {
  | Some(json) => Js.Json.decodeString(json)
  | _ => None
  };


module MakeServer = (DB: Database) => {
  let create = {
    let database = DB.create;
    DB.add(
      database,
      Comment.create(
      ~id=1, ~commenter=Guest({ name: "bob", url: None }),
      ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
      ~slug="lesleylai.info", ~parent_comment_id=None,
      ~text="Test the functionality of the comments")
    );

    DB.add(
      database,
      Comment.create(
      ~id=2, ~commenter=Guest({ name: "bob2", url: None }),
      ~date=Js.Date.makeWithYMD(~year=2020., ~month=2., ~date=1., ()),
      ~slug="lesleylai.info", ~parent_comment_id=Some(1),
      ~text="Reply")
    );

    let app = express();

    App.get(app, ~path="/comments/:slug") @@
      Middleware.from((next, req) =>
                      switch (getDictString(Request.params(req), "slug")) {
                      | Some(slug) => {
                          database
                            -> DB.filterBySlug(slug)
                          |> Json.Encode.list(Comment.Encode.comment)
                          |> Response.sendJson
                        }
                      | _ =>
                        next(Next.route);
                      }
    );

    let port =
      switch (Js.Nullable.toOption(Env.port)) {
      | Some(port) => int_of_string(port)
      | None => Js.log @@ "Cannot find PORT envirnment variable: default to 3000"; 3000
      };

    App.listen(app, ~port=port, ~onListen=(fun (e) =>
                                           switch (e) {
                                           | exception (Js.Exn.Error(e)) =>
                                             Js.log(e);
                                             Node.Process.exit(1);
                                           | _ => Printf.printf("Listening at localhost:%d", port)
                                           }), ())
  };
};

module Server = MakeServer(MockDatabase);

let server = Server.create;
