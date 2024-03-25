import React, { useState } from 'react'
import Modal, { ModalStyles } from '../../../../components/Modal/Modal'
import { CommentType } from '../../EventDetail'
import icons from '../../../../utils/icons'
import Textarea from '../../../../components/Textarea/Textarea'

import classes from "./CommentUpdateModal.module.css"
import Stars from '../../components/Stars/Stars'

type PropsType = {
    comment: CommentType
    onClose: () => void
    onSubmit: (inputs: CommentType) => void
}

const CommentUpdateModal = ({
    comment,
    onClose,
    onSubmit
}: PropsType) => {
    const [text, setText] = useState(comment.text)
    const [rating, setRating] = useState(comment.rating)
    const submit = () => {
        onSubmit({
            ...comment,
            text,
            rating
        })
    }

    const onChange: React.ChangeEventHandler<HTMLTextAreaElement> = (e) => {
        setText(e.target.value)
    }
  return (
      <Modal title="Update comment"
          textProceed="Save"
          textCancel="Cancel"
          onClose={onClose}
          onSubmit={submit}
          icon={icons.pen}
          disabled={text.length === 0}
      >
          <div className={classes.container}>
              <Textarea value={text} onChange={onChange} />
              <Stars rating={rating} setRating={setRating} />
          </div>
      </Modal>
  )
}

export default CommentUpdateModal