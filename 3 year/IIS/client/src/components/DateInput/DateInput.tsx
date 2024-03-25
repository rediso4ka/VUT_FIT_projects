import React, { SyntheticEvent, useRef } from 'react'
import ReactDatePicker from 'react-datepicker'

import classes from "./DateInput.module.css"
import InputLabel from '../InputLabel/InputLabel'
import { formatDate } from '../../utils/common'
import uuid from 'react-uuid'

export type DateChangeType = (name: string, date: Date | null) => void

type PropsType = {
    value: string,
    onChange?: DateChangeType,
    label?: string,
    name: string,
    required?: boolean
}

const DateInput = ({
    value,
    onChange: __onChange = () => { },
    label,
    name,
    required
}: PropsType) => {
    const ref = useRef<HTMLInputElement>(null)
    const id = uuid()
    const onChange = (date: Date | null, event: SyntheticEvent<any, Event> | undefined) => {
        __onChange(name, date)
    }
  return (
      <InputLabel required={required} htmlFor={id} value={label}>
          <div style={{ position: 'relative' }}>
              <input ref={ref} style={{ display: "none" }} />
              <ReactDatePicker
                  id={id}
                  name={name}
                  value={formatDate(value)}
                  onChange={onChange}
                  className={classes.input}
                  dateFormat="DD.MM.YYYY"
                //   readOnly
                  onKeyDown={(e) => e.preventDefault()}
              />
          </div>
      </InputLabel>
  )
}

export default DateInput