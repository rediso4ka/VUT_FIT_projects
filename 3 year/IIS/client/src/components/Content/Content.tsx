import React from 'react'
import classes from "./Content.module.css"
import classNames from 'classnames'

type PropsType = {
    children: React.ReactNode
    className?: string
}

const Content = ({
    children,
    className = ""
}: PropsType) => {
  return (
      <div className={classNames(classes.container, className)}>{children}</div>
  )
}

export default Content