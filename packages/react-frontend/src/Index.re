// Entry point

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

// All 4 examples.
ReactDOMRe.render(
  <BlinkingGreeting> {React.string("Hello!")} </BlinkingGreeting>,
  makeContainer("Blinking Greeting"),
);

ReactDOMRe.render(
  <ReducerFromReactJSDocs />,
  makeContainer("Reducer From ReactJS Docs"),
);

ReactDOMRe.render(
  <FetchedDogPictures />,
  makeContainer("Fetched Dog Pictures"),
);

module Styles = {
  open Css;

  let profile =
    style([
      width(px(50)),
      height(px(50)),
      borderRadius(pct(50.)),
      color(hex("fff")),
      backgroundColor(hex("512DA8")),
      fontSize(px(30)),
      textAlign(center),
      lineHeight(px(50)),
      margin(px(10)),
    ]);
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
        backgroundColor(hex("512DA8")),
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
  let make = (~children) => {
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
      <div style=profileImageAreaStyle>
        <ProfilePicture userName="Lesley" />
      </div>
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

    <div>
      <header>
        <nav style=navStyle>
          <div> {React.string("9 comments")} </div>
          <div> {React.string("Li Yu")} </div>
        </nav>
        <TextAreaWrapper>
          {React.string("Comments input area")}
        </TextAreaWrapper>
        <ul
          style={ReactDOMRe.Style.make(
            ~listStyle="none",
            ~padding="0",
            ~margin="0",
            (),
          )}>
          <li>
            <TextAreaWrapper> {React.string("Hello")} </TextAreaWrapper>
          </li>
          <li>
            <TextAreaWrapper> {React.string("World")} </TextAreaWrapper>
          </li>
        </ul>
      </header>
    </div>;
  };
};

ReactDOMRe.render(<Comments />, makeContainer("Show comments"));

// ReactDOMRe.render(
//   <ReasonUsingJSUsingReason />,
//   makeContainer("Reason Using JS Using Reason"),
// );
