open Belt;

type guest = {
  name: string,
  url: option(string)
};

type commenter = Anonymous | Guest(guest);

type t = {
  id: int,
  commenter: commenter,
  date: Js.Date.t,
  slug: string,
  parent_comment_id: option(int),
  text: string,
};

let create = (~id: int,
              ~commenter: commenter,
              ~date: Js.Date.t,
              ~slug: string,
              ~parent_comment_id: option(int),
              ~text: string) => {
  {
  id, commenter, date, slug, parent_comment_id, text
  }
};

let prependIf = (lst, condition, toAppend) => {
  if (condition) {
    [toAppend, ...lst]
  } else {
    lst
  }
};

module Encode = {
  open! Json.Encode;

  let commenter = (c: commenter) => {
    switch(c) {
    | Anonymous => null;
    | Guest({name, url}) =>
      let fields = [("name", string(name))]
      ->prependIf(Option.isSome(url), ("url", nullable(string, url)));
      object_(fields)
    };
  };

  let comment = (c: t) => {
    let fields = [
      ("id", int(c.id)),
      ("commenter", commenter(c.commenter)),
      ("date", date(c.date)),
      ("slug", string(c.slug)),
      ("text", string(c.text))]
      ->prependIf(Option.isSome(c.parent_comment_id),
                 ("parent_comment_id", nullable(int, c.parent_comment_id)));
    object_(fields)
  }
};

module Decode = {
  open! Json.Decode;

  let commenter = (json: Js.Json.t) => {
    oneOf([
    nullAs(Anonymous),
    (json) => Guest({
      name: json |> field("name", string),
      url: json |> optional(field("url", string))
    })
  ], json);
  };

  let comment = (json: Js.Json.t) => {
    {
    id: json |> field("id", int),
    commenter: json |> field("commenter", commenter),
    date: json |> field("date", date),
    slug: json |> field("slug", string),
    parent_comment_id: json |>
      optional(field("parent_comment_id", int)),
    text: json |> field("text", string),
    }
  };
};

let id = (comment: t) => comment.id;
let commenter = (comment: t) => comment.commenter;
let date = (comment: t) => comment.date;
let slug = (comment: t) => comment.slug;
let parent_comment_id = (comment: t) => comment.parent_comment_id;
let text = (comment: t) => comment.text;
