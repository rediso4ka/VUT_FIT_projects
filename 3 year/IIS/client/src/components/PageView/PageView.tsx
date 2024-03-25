import React from 'react'

import classes from "./PageView.module.css"
import classNames from 'classnames'

type PropsType = {
    children: React.ReactNode
    title: string
    scroll?: boolean
}

/**
 * 
 * @param param0 
 * @returns 
 */
const PageView = ({
    children,
    title,
    scroll
}: PropsType) => {
    const styles = classNames(classes.container, {
        [classes.scrollable]: scroll
    })
  return (
      <div className={styles}>
          <div className={classes.header}>
              <h2 className={classes.title}>{title}</h2>
          </div>
          {children}
      </div>
  )
}

export default PageView