open Jest;

open Comment;

let testComment = Comment.create(
  ~id=1, ~name="bob",
  ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
  ~slug="https://lesleylai.info", ~parent_comment_id=Some(1),
  ~text="Test the functionality of the comments");

describe("Comment.jsonfy should serialization comments into JSON", () => {
  open Expect;
  open! Expect.Operators;

  let json = Comment.jsonfy(testComment);

  test("id", () =>
       expect(Js.Dict.get(json, "id")
              ->Belt.Option.flatMap(Js.Json.decodeNumber)
              ->Belt.Option.map(int_of_float)) === Some(testComment->id)
  );

  test("name", () =>
       expect(Js.Dict.get(json, "name")
              ->Belt.Option.flatMap(Js.Json.decodeString))
       === Some(testComment->name)
  );

  test("date", () =>
       expect(Js.Dict.get(json, "date")
              ->Belt.Option.flatMap(Js.Json.decodeString))
       === Some((testComment->date) |> Js.Date.toString)
  );

  test("slug", () =>
       expect(Js.Dict.get(json, "slug")
              ->Belt.Option.flatMap(Js.Json.decodeString))
       === Some(testComment->slug)
  );

  test("parent_comment_id", () =>
       expect(Js.Dict.get(json, "parent_comment_id")
              ->Belt.Option.flatMap(Js.Json.decodeNumber)
              ->Belt.Option.map(int_of_float))
       === testComment->parent_comment_id
  );

  test("text", () =>
       expect(Js.Dict.get(json, "text")
              ->Belt.Option.flatMap(Js.Json.decodeString))
       === Some(testComment->text)
  );
});
