type t;

type guest = {
  name: string,
  url: option(string),
};

type commenter =
  | Guest(guest);

let getUserName: commenter => string;

type withParent = {
  comment: t,
  parent_id: option(int),
};

type withChildren = {
  comment: t,
  children: Js.Dict.t(withChildren)
};

let create:
  (
    ~commenter: commenter,
    ~date: Js.Date.t,
    ~slug: string,
    ~text: string
  ) =>
  t;

let createWithParent: (~comment: t, ~parent_id: option(int)) => withParent;
let createWithChildren: (~comment: t, ~children: Js.Dict.t(withChildren)) => withChildren;

module Encode: {
  let commenter: commenter => Js.Json.t;
  let comment: t => Js.Json.t;
  let commentWithParent: withParent => Js.Json.t;
  let commentWithParentDict: Js.Dict.t(withParent) => Js.Json.t;
  let commentWithChildren: withChildren => Js.Json.t;
  let commentWithChildrenDict: Js.Dict.t(withChildren) => Js.Json.t;
};

module Decode: {
  let commenter: Js.Json.t => commenter;
  let comment: Js.Json.t => t;
  let commentWithParent: Js.Json.t => withParent;
  let commentWithParentDict: Js.Json.t => Js.Dict.t(withParent);
  let commentWithChildren: Js.Json.t => withChildren;
  let commentWithChildrenDict: Js.Json.t => Js.Dict.t(withChildren);
};

// Getters
let commenter: t => commenter;
let date: t => Js.Date.t;
let slug: t => string;
let text: t => string;
