open Server;

module MyServer = MakeServer(MockDatabase);

let app = MyServer.create;

let port =
  switch (Js.Nullable.toOption(Env.port)) {
  | Some(port) => int_of_string(port)
  | None =>
    Js.log @@ "Cannot find PORT envirnment variable: default to 3000";
    3000;
  };

Express.App.listen(
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
