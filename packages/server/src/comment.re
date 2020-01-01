type t = {
  id: int,
  name: string,
  date: Js.Date.t,
  slug: string,
  parent_comment_id: option(int),
  text: string,
};

let jsonfyComment = (comment: t) => {
  let dict = Js.Dict.empty ();
  Js.Dict.set(dict, "id", comment.id |> float_of_int |> Js.Json.number);
  Js.Dict.set(dict, "name", Js.Json.string(comment.name));
  Js.Dict.set(dict, "date", Js.Json.string(comment.date |> Js.Date.toString));
  Js.Dict.set(dict, "slug", Js.Json.string(comment.slug));
  comment.parent_comment_id
    -> Belt.Option.map(float_of_int)
    -> Belt.Option.map(Js.Json.number)
    -> Belt.Option.map(Js.Dict.set(dict, "parent_comment_id"));
  Js.Dict.set(dict, "text", Js.Json.string(comment.text));
  dict
}
