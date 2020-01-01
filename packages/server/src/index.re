open Express;

let app = express();

let comments: array(Comment.t) = [|
Comment.create(
  ~id=1, ~name="bob",
  ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
  ~slug="https://lesleylai.info", ~parent_comment_id=Some(1),
  ~text="Test the functionality of the comments")
|];

/* make a common JSON object representing success */
let makeSuccessJson = () => {
  comments
  |> Array.map(Comment.jsonfy)
  |> Js.Json.objectArray
};


App.get(app, ~path="/comments") @@
Middleware.from((next, req) =>
  switch (Request.path(req)) {
  | "/comments" => Response.sendJson(makeSuccessJson())
  | s =>
    Js.log(s);
    next(Next.route);
  }
);

let server = App.listen(app, ~port=3000, ~onListen=(fun (e) =>
 switch (e) {
 | exception (Js.Exn.Error(e)) =>
   Js.log(e);
   Node.Process.exit(1);
 | _ => Js.log @@ "Listening at localhost:3000"
 }), ());
