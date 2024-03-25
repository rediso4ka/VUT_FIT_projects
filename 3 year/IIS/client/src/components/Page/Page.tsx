import React from 'react'
import classes from "./Page.module.css"

type PropsType = {
    children: React.ReactNode
}

/**
 * 
 * @param param0 
 * @returns 
 */
const Page = ({ children }: PropsType) => {
  return (
      <div className={classes.container}>{children}</div>
  )
}

export default Page