import React from 'react'
import classes from "./Container.module.css"
import uuid from 'react-uuid'

type PropsType = {
    children: React.ReactNode
}

export const containerId: string = "pg-container"

const Container = ({ children }: PropsType) => {
    return (
        <div id={containerId} className={classes.container}>{children}</div>
    )
}

export default Container