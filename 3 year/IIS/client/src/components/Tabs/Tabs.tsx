import React from 'react'
import { Link, useLocation } from 'react-router-dom'

import classes from "./Tabs.module.css"
import classNames from 'classnames'

export type TabsType = {
    to: string,
    text: string
}[]

type PropsType = {
    tabs: TabsType
    prefix: string
}

const Tabs = ({
    tabs,
    prefix
}: PropsType) => {
    const location = useLocation()

    const isActive = (to: string) => {
        return location.pathname === (to ? prefix ? prefix + '/' + to : to : prefix)
    }

  return (
      <div className={classes.tabs}>
          {tabs.map(({ to, text }) => <Link key={to} to={to} className={classNames(classes.tab, { [classes.active]: isActive(to) })}>{text}</Link>)}
      </div>
  )
}

export default Tabs