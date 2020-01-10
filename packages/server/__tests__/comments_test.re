open Jest;

open Comment;

describe("commenter serialization", () => {
  open Expect;
  open! Expect.Operators;

  let anonymous = Anonymous;
  test("anonymous", () =>
       expect(anonymous) == anonymous
       ->Comment.Encode.commenter
       ->Comment.Decode.commenter);

  let guest = Guest({ name: "bob", url: None });
  test("guest", () =>
       expect(guest) == guest
       ->Comment.Encode.commenter
       ->Comment.Decode.commenter);

  let guest2 = Guest({ name: "bob", url: Some("bob.com") });
  test("guest with name", () =>
       expect(guest2) == guest2
       ->Comment.Encode.commenter
       ->Comment.Decode.commenter);
});

describe("comment serialization", () => {
  open Expect;
  open! Expect.Operators;

  let comment1 = Comment.create(
    ~id=1, ~commenter=Guest({ name: "bob", url: None }),
    ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
    ~slug="https://lesleylai.info", ~parent_comment_id=None,
    ~text="Test the functionality of the comments");
  test("comment1", () =>
       expect(comment1) == comment1
       ->Comment.Encode.comment
       ->Comment.Decode.comment);

  let comment2 = Comment.create(
    ~id=1, ~commenter=Anonymous,
    ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=2., ()),
    ~slug="https://lesleylai.info", ~parent_comment_id=Some(1),
    ~text="Test the functionality of the comments");
  test("comment2", () =>
       expect(comment2) == comment2
       ->Comment.Encode.comment
       ->Comment.Decode.comment);
});
