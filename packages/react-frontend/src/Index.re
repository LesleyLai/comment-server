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

module ProfilePicture = {
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
    let textAreaStyle =
      ReactDOMRe.Style.make(
        ~display="flex",
        ~flexDirection="row",
        ~borderBottom="1px solid red",
        (),
      );

    let profileImageAreaStyle =
      ReactDOMRe.Style.make(~width="100px", ~borderRight="1px solid red", ());

    let remainingAreaStyle = ReactDOMRe.Style.make(~flexGrow="1", ());

    <div style=textAreaStyle>
      <div style=profileImageAreaStyle> <ProfilePicture userName /> </div>
      <div style=remainingAreaStyle> children </div>
    </div>;
  };
};

module Comments = {
  [@react.component]
  let make = () => {
    let navStyle =
      ReactDOMRe.Style.make(
        ~display="flex",
        ~justifyContent="space-between",
        ~borderBottom="2px solid lightgrey",
        ~fontSize="15px",
        (),
      );

    let (comments, setComments) = React.useState(() => [||]);

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
        |> then_(Fetch.Response.text)
        |> then_((text: string) =>
             setComments(_ =>
               text |> Js.Json.parseExn |> Comment.Decode.comments
             )
             |> resolve
           )
      );

    <div>
      <header>
        <nav style=navStyle>
          <div> {React.string("9 comments")} </div>
          <div> {React.string("Lesley")} </div>
        </nav>
        <TextAreaWrapper userName="Li Yu">
          {React.string("Comments input area")}
        </TextAreaWrapper>
        <ul
          style={ReactDOMRe.Style.make(
            ~listStyle="none",
            ~padding="0",
            ~margin="0",
            (),
          )}>
          {ReasonReact.array(
             {comments
              |> Array.map(comment => {
                   let userName =
                     switch (comment->Comment.commenter) {
                     | Comment.Anonymous => "anonymous"
                     | Comment.Guest(guest) => guest.name
                     };

                   <li key={comment->Comment.id |> string_of_int}>
                     <TextAreaWrapper userName>
                       {React.string(comment->Comment.text)}
                     </TextAreaWrapper>
                   </li>;
                 })},
           )}
        </ul>
      </header>
    </div>;
  };
};

ReactDOMRe.render(<Comments />, makeContainer("Show comments"));
