open Jest;

open Comment;

describe("commenter serialization", () => {
  open Expect;
  open! Expect.Operators;

  let guest = Guest({name: "bob", url: None});
  test("guest", () =>
    expect(guest) == guest->Comment.Encode.commenter->Comment.Decode.commenter
  );

  let guest2 = Guest({name: "bob", url: Some("bob.com")});
  test("guest with name", () =>
    expect(guest2)
    == guest2->Comment.Encode.commenter->Comment.Decode.commenter
  );
});

let comment1 =
  Comment.create(
    ~commenter=Guest({name: "bob", url: None}),
    ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=1., ()),
    ~slug="https://lesleylai.info",
    ~text="Test the functionality of the comments",
  );

let comment2 =
  Comment.create(
    ~commenter=Guest({name: "bob2", url: None}),
    ~date=Js.Date.makeWithYMD(~year=2020., ~month=1., ~date=2., ()),
    ~slug="https://lesleylai.info",
    ~text="Test the functionality of the comments",
  );

describe("comment serialization", () => {
  open Expect;
  open! Expect.Operators;

  test("comment1", () =>
    expect(comment1)
    == comment1->Comment.Encode.comment->Comment.Decode.comment
  );

  test("comment2", () =>
    expect(comment2)
    == comment2->Comment.Encode.comment->Comment.Decode.comment
  );
});

describe("comments with parent serialization", () => {
  open Expect;
  open! Expect.Operators;

  let commentWP1 =
    Comment.createWithParent(~comment=comment1, ~parent_id=None);

  test("commentWP1", () =>
    expect(commentWP1)
    == commentWP1
       ->Comment.Encode.commentWithParent
       ->Comment.Decode.commentWithParent
  );

  let commentWP2 =
    Comment.createWithParent(~comment=comment1, ~parent_id=Some(1));

  test("commentWP2", () =>
    expect(commentWP2)
    == commentWP2
       ->Comment.Encode.commentWithParent
       ->Comment.Decode.commentWithParent
  );

  let dict = Js.Dict.fromList([("0", commentWP1), ("1", commentWP2)]);
  test("commentWP dict", () =>
    expect(dict)
    == dict
       ->Comment.Encode.commentWithParentDict
       ->Comment.Decode.commentWithParentDict
  );
});

describe("comments with children serialization", () => {
  open Expect;
  open! Expect.Operators;

  let children =
    Js.Dict.fromList([
      (
        "1",
        Comment.createWithChildren(
          ~comment=comment2,
          ~children=Js.Dict.empty(),
        ),
      ),
    ]);
  let commentWC = Comment.createWithChildren(~comment=comment1, ~children);
  test("comment with children", () =>
    expect(commentWC)
    == commentWC
       ->Comment.Encode.commentWithChildren
       ->Comment.Decode.commentWithChildren
  );

  let dict = Js.Dict.fromList([("0", commentWC)]);
  test("comments with children dictionary", () =>
    expect(dict)
    == dict
       ->Comment.Encode.commentWithChildrenDict
       ->Comment.Decode.commentWithChildrenDict
  );
});