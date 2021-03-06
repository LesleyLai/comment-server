open Css;

// Create color hue from hashing a string
let hueFromString = s => {
  let hash = ref(0);
  s |> String.iter(c => {hash := Char.code(c) + hash^ lsl 5 - hash^});

  (hash^ mod 360)->float_of_int;
};

let comments = style([fontFamily("sans-serif")]);

let commentsCount = style([fontSize(px(18))]);

let textArea =
  style([display(flexBox), flexDirection(row), padding(px(5))]);

let commentEditor =
  style([
    resize(`none),
    minHeight(px(100)),
    padding(px(10)),
    marginBottom(px(5)),
    border(px(1), `solid, hex("AAA")),
    borderRadius(px(5)),
    selector(":focus", [borderColor(hex("333")), outlineStyle(`none)]),
    selector(
      "::placeholder",
      [fontSize(px(18)), color(hex("687a86")), lineHeight(px(30))],
    ),
  ]);

let profileImageArea = style([width(px(75))]);

let profileImage = (~diameter, ~userName) => {
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
};

let remainingArea =
  style([flexGrow(1.), display(flexBox), flexDirection(column)]);

let nav =
  style([
    display(flexBox),
    justifyContent(spaceBetween),
    borderBottom(px(2), `solid, lightgrey),
    fontSize(px(15)),
  ]);

let commentsList =
  style([listStyleType(`none), padding(zero), margin(zero)]);

let commentHeader =
  style([
    display(flexBox),
    flexDirection(row),
    borderTop(px(1), `solid, hex("bbb")),
    paddingTop(px(5)),
  ]);

let commentBody = style([margin2(~v=px(10), ~h=zero)]);

let commenter = style([fontWeight(`semiBold)]);
let date = style([fontWeight(`light), color(hex("666"))]);
let bullet = style([padding2(~v=zero, ~h=px(5))]);

let replyToggle =
  style([
    fontWeight(`semiBold),
    textDecoration(`none),
    color(hex("008888")),
    selector(":hover", [color(hex("003333"))]),
  ]);

let authArea = style([display(flexBox), justifyContent(spaceBetween)]);
let submitAreaForUsers = style([display(flexBox), justifyContent(flexEnd)]);

let authAreaInputs =
  style([
    display(flexBox),
    justifyContent(spaceBetween),
    selector(
      "input",
      [
        padding(px(5)),
        margin2(~v=`zero, ~h=px(3)),
        border(px(1), `solid, hex("aaa")),
        borderRadius(px(2)),
        outlineStyle(`none),
      ],
    ),
  ]);

let submitButton =
  style([
    backgroundColor(hex("CCC")),
    padding2(~v=px(5), ~h=px(10)),
    selector(":not(:disabled):hover", [backgroundColor(hex("AAA"))]),
    selector(
      ":focus",
      [border(px(1), `solid, hex("333")), outlineStyle(`none)],
    ),
  ]);