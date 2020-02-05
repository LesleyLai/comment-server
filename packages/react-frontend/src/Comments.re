open Common;

module ProfileImage = {
  [@react.component]
  let make = (~userName: string, ~diameter=50) => {
    <div className={Style.profileImage(~diameter, ~userName)}>
      {userName->String.sub(0, 1) |> React.string}
    </div>;
  };
};

module TextAreaWrapper = {
  [@react.component]
  let make = (~children, ~userName) => {
    <div className=Style.textArea>
      <div className=Style.profileImageArea> <ProfileImage userName /> </div>
      <div className=Style.remainingArea> children </div>
    </div>;
  };
};

module CommentInputArea = {
  let postComment = (~userName, ~commentText, ~parentId=?, ()) => {
    let comment1 =
      Comment.create(
        ~commenter=Guest({name: userName, url: None}),
        ~date=Js.Date.fromFloat(Js.Date.now()),
        ~slug="lesleylai.info",
        ~text=commentText,
      );

    let withParent =
      Comment.createWithParent(~comment=comment1, ~parent_id=parentId);

    let json = Comment.Encode.commentWithParent(withParent);

    let _ =
      Fetch.fetchWithInit(
        "http://127.0.0.1:3000/comment",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~headers=
            Fetch.HeadersInit.make({"Content-Type": "application/json"}),
          ~body=Fetch.BodyInit.make(Js.Json.stringify(json)),
          (),
        ),
      );

    ();
  };

  [@react.component]
  let make = (~userName, ~parentId=?) => {
    let (commentText, setCommentText) = React.useState(() => "");
    <TextAreaWrapper userName>
      <textarea
        className=Style.commentEditor
        placeholder="Join the discussion..."
        onChange={event =>
          setCommentText(ReactEvent.Form.target(event)##value)
        }>
        {commentText |> React.string}
      </textarea>
      <button
        onClick={_ => postComment(~userName, ~commentText, ~parentId?, ())}>
        {Printf.sprintf("post as %s", userName) |> React.string}
      </button>
    </TextAreaWrapper>;
  };
};

module type CommentAreaType = {
  [@react.component]
  let make:
    (
      ~commentId: int,
      ~withChildren: Comment.withChildren,
      ~userName: string
    ) =>
    React.element;
};

module rec CommentArea: CommentAreaType = {
  [@react.component]
  let make = (~commentId: int, ~withChildren: Comment.withChildren, ~userName) => {
    let (replyToggle, setReplyToggle) = React.useState(() => false);

    let comment = withChildren.comment;
    let commenterUserName =
      switch (comment->Comment.commenter) {
      | Comment.Guest(guest) => guest.name
      };

    <TextAreaWrapper userName=commenterUserName>
      <header className=Style.commentHeader>
        <span> {commenterUserName |> React.string} </span>
        <span className=Style.bullet> {{js|•|js} |> React.string} </span>
        <span>
          {comment->Comment.date |> Js.Date.toDateString |> React.string}
        </span>
      </header>
      <div className=Style.commentBody>
        {React.string(comment->Comment.text)}
      </div>
      <footer>
        <a
          href="#"
          onClick={e => {
            e->ReactEvent.Synthetic.preventDefault;
            setReplyToggle(_ => !replyToggle);
          }}>
          {"reply" |> React.string}
        </a>
        {replyToggle
           ? <CommentInputArea userName parentId=commentId /> : React.null}
        <ul className=Style.commentsList>
          {ReasonReact.array(
             withChildren.children
             |> Js.Dict.entries
             |> Array.map(((id, child)) => {
                  <li key=id>
                    <CommentArea
                      commentId={int_of_string(id)}
                      userName
                      withChildren=child
                    />
                  </li>
                }),
           )}
        </ul>
      </footer>
    </TextAreaWrapper>;
  };
};

[@react.component]
let make = () => {
  let (comments, setComments) = React.useState(() => Js.Dict.empty());

  let _ =
    Js.Promise.(
      Fetch.fetchWithInit(
        "http://127.0.0.1:3000/comments/lesleylai.info",
        Fetch.RequestInit.make(
          ~method_=Get,
          ~headers=
            Fetch.HeadersInit.make({"Content-Type": "application/json"}),
          (),
        ),
      )
      |> then_(Fetch.Response.json)
      |> then_((text: Js.Json.t) =>
           setComments(_ => text |> Comment.Decode.commentWithChildrenDict)
           |> resolve
         )
    );

  let userName = "Li Yu";

  <div>
    <header>
      <nav className=Style.nav>
        <div>
          {Printf.sprintf(
             "%i comments",
             comments |> Js.Dict.entries |> Array.length,
           )
           |> React.string}
        </div>
        <div> {userName |> React.string} </div>
      </nav>
      <CommentInputArea userName />
      <ul className=Style.commentsList>
        {ReasonReact.array(
           comments
           |> Js.Dict.entries
           |> Array.map(((id, withChildren)) => {
                <li key=id>
                  <CommentArea
                    commentId={int_of_string(id)}
                    userName
                    withChildren
                  />
                </li>
              }),
         )}
      </ul>
    </header>
  </div>;
};
