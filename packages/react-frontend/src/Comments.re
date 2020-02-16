open Common;

module ProfileImage = {
  [@react.component]
  let make = (~user, ~diameter=50) => {
    let userName = Comment.getUserName(user);
    <div className={Style.profileImage(~diameter, ~userName)}>
      {userName->String.sub(0, 1) |> React.string}
    </div>;
  };
};

module TextAreaWrapper = {
  [@react.component]
  let make = (~children, ~user=?) => {
    <div className=Style.textArea>
      {switch (user) {
       | None => React.null
       | Some(u) =>
         <div className=Style.profileImageArea> <ProfileImage user=u /> </div>
       }}
      <div className=Style.remainingArea> children </div>
    </div>;
  };
};

module CommentInputArea = {
  let postComment = (~user, ~commentText, ~parentId=?, ()) => {
    let comment1 =
      Comment.create(
        ~commenter=user,
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
  let make = (~user=?, ~parentId=?) => {
    let (commentText, setCommentText) = React.useState(() => "");
    <TextAreaWrapper ?user>
      <textarea
        className=Style.commentEditor
        placeholder="Join the discussion..."
        onChange={event =>
          setCommentText(ReactEvent.Form.target(event)##value)
        }>
        {commentText |> React.string}
      </textarea>
      {switch (user) {
       | None =>
        let user = Comment.Guest({name: "Bob", url: None});
         <>
           <label>
             {"Username: " |> React.string}
             <input type_="text" />
           </label>
           <label> 
            {"URL: " |> React.string} 
            <input type_="text" /> 
          </label>
          <button
           disabled={String.length(commentText) == 0}
           onClick={_ => postComment(~user, ~commentText, ~parentId?, ())}>
           {Printf.sprintf("post") |> React.string}
         </button>
         </>
       | Some(u) =>
         <button
           disabled={String.length(commentText) == 0}
           onClick={_ => postComment(~user=u, ~commentText, ~parentId?, ())}>
           {Printf.sprintf("post as %s", Comment.getUserName(u))
            |> React.string}
         </button>
       }}
    </TextAreaWrapper>;
  };
};

module type CommentAreaType = {
  [@react.component]
  let make:
    (
      ~commentId: int,
      ~withChildren: Comment.withChildren,
      ~user: Comment.commenter =?
    ) =>
    React.element;
};

module rec CommentArea: CommentAreaType = {
  [@react.component]
  let make = (~commentId: int, ~withChildren: Comment.withChildren, ~user=?) => {
    let (replyToggle, setReplyToggle) = React.useState(() => false);

    let comment = withChildren.comment;
    let commenter = comment->Comment.commenter;

    <TextAreaWrapper user=commenter>
        <header className=Style.commentHeader>
          <span> {commenter |> Comment.getUserName |> React.string} </span>
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
          {replyToggle ? <CommentInputArea ?user parentId=commentId /> : React.null}
          <ul className=Style.commentsList>
            {ReasonReact.array(
               withChildren.children
               |> Js.Dict.entries
               |> Array.map(((id, child)) => {
                    <li key=id>
                      <CommentArea
                        commentId={int_of_string(id)}
                        ?user
                        withChildren=child
                      />
                    </li>
                  }),
             )}
          </ul>
        </footer>
      </TextAreaWrapper>; // TODO: propagate user
  };
};

[@react.component]
let make = () => {
  let (comments, setComments) = React.useState(() => Js.Dict.empty());
  let (user, setUser) = React.useState(() => None);
  // let (user, setUser) = React.useState(() => Some(Comment.Guest({name: "Lesley", url: None})));

  let userName = Belt.Option.map(user, Comment.getUserName);

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
          {userName->Belt.Option.mapWithDefault(React.null, React.string)}
        </nav>
        <CommentInputArea ?user />
      </header>
      <ul className=Style.commentsList>
        {ReasonReact.array(
           comments
           |> Js.Dict.entries
           |> Array.map(((id, withChildren)) => {
                <li key=id>
                  <CommentArea
                    commentId={int_of_string(id)}
                    ?user
                    withChildren
                  />
                </li>
              }),
         )}
      </ul>
  </div>;
};