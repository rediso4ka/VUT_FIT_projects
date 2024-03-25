import React from 'react'
import classes from "./Textarea.module.css"
import InputLabel from '../InputLabel/InputLabel'
import uuid from 'react-uuid'

type PropsType = {
    value: string,
    onChange?: React.ChangeEventHandler<HTMLTextAreaElement>,
    label?: string,
    name?: string
    readOnly?: boolean
}

const Textarea = ({
    value,
    onChange,
    label,
    name,
    readOnly
}: PropsType) => {
    const id = uuid()
    return (
        <InputLabel htmlFor={id} value={label}>
          <textarea
                id={id}
              name={name}
              value={value}
              onChange={onChange}
              className={classes.input}
              readOnly={readOnly}
          ></textarea>
      </InputLabel>
  )
}

export default Textarea