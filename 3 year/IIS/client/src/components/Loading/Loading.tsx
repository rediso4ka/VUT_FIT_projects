import React from 'react'

import classes from "./Loading.module.css"
import { FloatingPortal } from '@floating-ui/react'
import { floatingRoot } from '../../context/AppContextProvider'

const Loading = () => {
  return (
    <FloatingPortal root={floatingRoot}>
      <div className={classes.loading}><div></div><div></div><div></div><div></div></div>
    </FloatingPortal>
  )
}

export default Loading