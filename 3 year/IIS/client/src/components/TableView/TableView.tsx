import React from 'react'
import classes from "./TableView.module.css"

type PropsType = {
    children: React.ReactNode
}

const TableView = ({
    children
}: PropsType) => {
  return (
      <div className={classes.container}>{children}</div>
  )
}

export default TableView