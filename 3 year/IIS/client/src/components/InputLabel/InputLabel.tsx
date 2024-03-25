import React from 'react'
import classes from "./InputLabel.module.css"
import StarRequire from '../StarRequire/StarRequire'
import classNames from 'classnames'

type PropsType = {
    htmlFor?: string,
    children: React.ReactNode,
    value?: string // TODO add vertical alignment
    required?: boolean,
    flexStart?: boolean
}

const InputLabel = ({
    htmlFor,
    children,
    value,
    required,
    flexStart
}: PropsType, ref: React.Ref<HTMLLabelElement>) => {
    if (!value) {
        return (
            <div>
                {children}
            </div>
        )
    }

    return (
        <>
            <label ref={ref} className={classes.label} htmlFor={htmlFor ?? classes.container}>
                {value}
                {required && <StarRequire />}
            </label>
            {children}
        </>
    )
}

export default React.forwardRef(InputLabel)