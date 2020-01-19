open Css;

let textArea =
  style([
    display(flexBox),
    flexDirection(row),
    borderBottom(px(1), `solid, red),
  ]);

let commentEditor =
  style([resize(`vertical), width(pct(100.)), height(px(100))]);

let profileImageArea =
  style([width(px(100)), borderRight(px(1), `solid, red)]);

let remainingArea = style([flexGrow(1.)]);

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
  style([display(flexBox), flexDirection(row)]);

let commentBody =
  style([margin2(~v=px(10), ~h=zero)]);

let bullet =
  style([padding2(~v=zero, ~h=px(5))]);