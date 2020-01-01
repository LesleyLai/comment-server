open Jest;

open Comment;

let testComment = {
  id: 1,
  name: "Bob",
  date: Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
  slug: "https://lesleylai.info",
  parent_comment_id: None,
  text: "Test the functionality of the comments"
}

describe("jsonfyComment", () => {
  open Expect;
  open! Expect.Operators;

  let json = jsonfyComment(testComment);

  test("id", () =>
       expect(Js.Dict.get(json, "id")
              ->Belt.Option.flatMap(Js.Json.decodeNumber)
              ->Belt.Option.map(int_of_float)) === Some(testComment.id)
  )
});
