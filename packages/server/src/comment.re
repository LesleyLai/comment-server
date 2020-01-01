type t = {
  id: int,
  name: string,
  date: Js.Date.t,
  slug: string,
  parent_comment_id: option(int),
  text: string,
};

let create = (~id: int,
              ~name: string,
              ~date: Js.Date.t,
              ~slug: string,
              ~parent_comment_id: option(int),
              ~text: string) => {
  {
  id, name, date, slug, parent_comment_id, text
}
}

let jsonfy = (comment: t) => {
  let dict = Js.Dict.empty ();
  Js.Dict.set(dict, "id", comment.id |> float_of_int |> Js.Json.number);
  Js.Dict.set(dict, "name", Js.Json.string(comment.name));
  Js.Dict.set(dict, "date", Js.Json.string(comment.date |> Js.Date.toString));
  Js.Dict.set(dict, "slug", Js.Json.string(comment.slug));
  let _ = comment.parent_comment_id
    -> Belt.Option.map(float_of_int)
    -> Belt.Option.map(Js.Json.number)
    -> Belt.Option.map(Js.Dict.set(dict, "parent_comment_id"));
  Js.Dict.set(dict, "text", Js.Json.string(comment.text));
  dict
}

let id = (comment: t) => comment.id;
let name = (comment: t) => comment.name;
let date = (comment: t) => comment.date;
let slug = (comment: t) => comment.slug;
let parent_comment_id = (comment: t) => comment.parent_comment_id;
let text = (comment: t) => comment.text;
