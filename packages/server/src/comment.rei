type t;

type guest = {
  name: string,
  url: option(string)
};

type commenter = Anonymous | Guest(guest);

let create: (~id: int,
             ~commenter: commenter,
             ~date: Js.Date.t,
             ~slug: string,
             ~parent_comment_id: option(int),
             ~text: string) => t;

module Encode: {
  let commenter: commenter => Js.Json.t;
  let comment: t => Js.Json.t;
};

module Decode: {
  let commenter: Js.Json.t => commenter;
  let comment: Js.Json.t => t;
}

// Getters
let id: t => int;
let commenter: t => commenter;
let date: t => Js.Date.t;
let slug: t => string;
let parent_comment_id: t => option(int);
let text: t => string;
