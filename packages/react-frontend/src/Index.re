// Entry point
open Common;

[@bs.val] external document: Js.t({..}) = "document";

// We're using raw DOM manipulations here, to avoid making you read
// ReasonReact when you might precisely be trying to learn it for the first
// time through the examples later.
let gstyles = document##createElement("style");
document##head##appendChild(gstyles);
gstyles##innerHTML #= ExampleStyles.style;

let makeContainer = text => {
  let container = document##createElement("div");
  container##className #= "container";

  let title = document##createElement("div");
  title##className #= "containerTitle";
  title##innerText #= text;

  let content = document##createElement("div");
  content##className #= "containerContent";

  let () = container##appendChild(title);
  let () = container##appendChild(content);
  let () = document##body##appendChild(container);

  content;
};

// Create color hue from hashing a string
let hueFromString = s => {
  let hash = ref(0);
  s |> String.iter(c => {hash := Char.code(c) + hash^ lsl 5 - hash^});

  (hash^ mod 360)->float_of_int;
};

module ProfileImage = {
  [@react.component]
  let make = (~userName: string, ~diameter=50) => {
    open Css;
    let css =
      style([
        width(px(diameter)),
        height(px(diameter)),
        borderRadius(pct(50.)),
        color(hex("fff")),
        backgroundColor(hsl(deg(userName->hueFromString), 100., 30.)),
        fontSize(px(30)),
        textAlign(center),
        lineHeight(px(diameter)),
        margin(px(10)),
      ]);

    <div className=css> {userName->String.sub(0, 1) |> React.string} </div>;
  };
};

module TextAreaWrapper = {
  [@react.component]
  let make = (~children, ~userName) => {
    <div className=Style.textArea>
      <div className=Style.profileImageArea> <ProfileImage userName /> </div>
      <div className=Style.remainingArea> children </div>
    </div>;
  };
};

module CommentInputArea = {
  [@react.component]
  let make = (~userName) => {
    <TextAreaWrapper userName>
      <textarea className=Style.commentEditor />
      <button>
        {Printf.sprintf("post as %s", userName) |> React.string}
      </button>
    </TextAreaWrapper>;
  };
};

module CommentArea = {
  [@react.component]
  let make = (~comment, ~userName) => {
    let (replyToggle, setReplyToggle) = React.useState(() => false);

    let commenterUserName =
      switch (comment->Comment.commenter) {
      | Comment.Guest(guest) => guest.name
      };

    <TextAreaWrapper userName=commenterUserName>
      <header className=Style.commentHeader>
        <span> {commenterUserName |> React.string} </span>
        <span className=Style.bullet> {"*" |> React.string} </span>
        <span>
          {comment->Comment.date |> Js.Date.toDateString |> React.string}
        </span>
      </header>
      <div className=Style.commentBody>
        {React.string(comment->Comment.text)}
      </div>
      <footer>
        <a href="#" onClick={_ => setReplyToggle(_ => !replyToggle)}>
          {"reply" |> React.string}
        </a>
        {replyToggle ? <CommentInputArea userName /> : React.null}
      </footer>
    </TextAreaWrapper>;
  };
};

module Comments = {
  [@react.component]
  let make = () => {
    let (comments, setComments) = React.useState(() => Js.Dict.empty());

    let _ =
      Js.Promise.(
        Fetch.fetchWithInit(
          "http://127.0.0.1:3000/comments/lesleylai.info",
          Fetch.RequestInit.make(
            ~method_=Get,
            ~headers=
              Fetch.HeadersInit.make({"Content-Type": "application/json"}),
            (),
          ),
        )
        |> then_(Fetch.Response.json)
        |> then_((text: Js.Json.t) =>
             setComments(_ => text |> Comment.Decode.commentWithChildrenDict)
             |> resolve
           )
      );

    let userName = "Li Yu";

    <div>
      <header>
        <nav className=Style.nav>
          <div>
            {Printf.sprintf(
               "%i comments",
               comments |> Js.Dict.entries |> Array.length,
             )
             |> React.string}
          </div>
          <div> {userName |> React.string} </div>
        </nav>
        <CommentInputArea userName />
        <ul className=Style.commentsList>
          {ReasonReact.array(
             comments
             |> Js.Dict.entries
             |> Array.map(((id, withChildren: Comment.withChildren)) => {
                  <li key=id>
                    <CommentArea userName comment={withChildren.comment} />
                  </li>
                }),
           )}
        </ul>
      </header>
    </div>;
  };
};

ReactDOMRe.render(<Comments />, makeContainer("Show comments"));
