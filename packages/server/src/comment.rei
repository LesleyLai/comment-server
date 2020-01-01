type t;

let create: (~id: int,
             ~name: string,
             ~date: Js.Date.t,
             ~slug: string,
             ~parent_comment_id: option(int),
             ~text: string) => t;

let jsonfy: t => Js.Dict.t(Js.Json.t);

// Getters
let id: t => int;
let name: t => string;
let date: t => Js.Date.t;
let slug: t => string;
let parent_comment_id: t => option(int);
let text: t => string;
