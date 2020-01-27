open Belt;

type guest = {
  name: string,
  url: option(string),
};

type commenter =
  | Guest(guest);

type t = {
  commenter,
  date: Js.Date.t,
  slug: string,
  text: string,
};

type withParent = {
  comment: t,
  parent_id: option(int),
};

type withChildren = {
  comment: t,
  children: Js.Dict.t(withChildren)
};

let create =
    (~commenter: commenter, ~date: Js.Date.t, ~slug: string, ~text: string) => {
  {commenter, date, slug, text};
};

let createWithParent = (~comment: t, ~parent_id: option(int)) => {
  {comment, parent_id};
};

let createWithChildren = (~comment: t, ~children: Js.Dict.t(withChildren)) => {
  {comment, children};
};

module Encode = {
  open! Json.Encode;

  let commenter = (c: commenter) => {
    switch (c) {
    | Guest({name, url}) =>
      let fields =
        [("name", string(name))]
        ->ListUtil.addIf(
            Option.isSome(url),
            ("url", nullable(string, url)),
          );
      object_(fields);
    };
  };

  let comment = (c: t) => {
    let fields = [
      ("commenter", commenter(c.commenter)),
      ("date", date(c.date)),
      ("slug", string(c.slug)),
      ("text", string(c.text)),
    ];
    object_(fields);
  };

  let commentWithParent = (w: withParent) => {
    let fields =
      [("comment", comment(w.comment))]
      ->ListUtil.addIf(
          Option.isSome(w.parent_id),
          ("parent_id", nullable(int, w.parent_id)),
        );
    object_(fields);
  };

  let commentWithParentDict = dict(commentWithParent);

  let rec commentWithChildren = (w: withChildren) => {
    let fields =
      [("comment", comment(w.comment))]
      ->ListUtil.addIf(
      w.children -> Js.Dict.entries -> Array.length != 0,
      ("children", dict(commentWithChildren, w.children))
    );
    object_(fields);
  };
  
  let commentWithChildrenDict = dict(commentWithChildren);
};

module Decode = {
  open! Json.Decode;

  let commenter = (json: Js.Json.t) => {
    Guest({
      name: json |> field("name", string),
      url: json |> optional(field("url", string)),
    });
  };

  let comment = (json: Js.Json.t) => {
    {
      commenter: json |> field("commenter", commenter),
      date: json |> field("date", date),
      slug: json |> field("slug", string),
      text: json |> field("text", string),
    };
  };

  let commentWithParent = (json: Js.Json.t) => {
    {
      comment: json |> field("comment", comment),
      parent_id: json |> optional(field("parent_id", int)),
    };
  };

  let commentWithParentDict = dict(commentWithParent);

  let rec commentWithChildren = (json: Js.Json.t) => {
    {
      comment: json |> field("comment", comment),
      children: (json
                |> optional(field("children", dict(commentWithChildren))))
                -> Option.getWithDefault(Js.Dict.empty())
    }
  };

  let commentWithChildrenDict = dict(commentWithChildren);
};

let commenter = (comment: t) => comment.commenter;
let date = (comment: t) => comment.date;
let slug = (comment: t) => comment.slug;
let text = (comment: t) => comment.text;
