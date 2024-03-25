import React from 'react'
import classes from "./Input.module.css"
import InputLabel from '../InputLabel/InputLabel'
import classNames from 'classnames'
import uuid from 'react-uuid'

type PropsType = {
    value: string,
    type?: string,
    onChange?: React.ChangeEventHandler<HTMLInputElement>,
    label?: string,
    name?: string,
    inactive?: boolean
    className?: string
    placeholder?: string
    required?: boolean
    min?: number
}

const Input = ({
    value,
    type,
    onChange,
    label,
    name,
    inactive,
    className = "",
    placeholder = "",
    required,
    min

}: PropsType) => {
    const id = uuid()
    return (
        <InputLabel required={required} htmlFor={id} value={label}>
            <input
                min={min}
                id={id}
                name={name}
                type={type}
                value={value}
                onChange={onChange}
                placeholder={placeholder}
                className={classNames(classes.input, className)}
                readOnly={inactive}
            />
        </InputLabel>
    )
}

export default Input