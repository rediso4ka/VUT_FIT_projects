import React, { useContext } from 'react'
import { Link } from 'react-router-dom'

import classes from "./Menu.module.css"
import { AppContext } from '../../context/AppContextProvider'
import { roles } from '../../utils/common'
import { Icon } from '@iconify/react'
import icons from '../../utils/icons'
import Button from '../Button/Button'

type Props = {
    setIsActive: React.Dispatch<React.SetStateAction<boolean>>
}

const Menu = ({
    setIsActive
}: Props, ref: React.Ref<HTMLDivElement>) => {
    const context = useContext(AppContext)
  return (
      <aside ref={ref} className={classes.container}>
          <Button alignLeft to="/">Home</Button>
          {context.isAuth && (
              <Button alignLeft to="/tickets">My tickets</Button>
          )}
          {(context.isAuth && context.user && context.user.role !== roles.USER) && (
              <>
                  <Button alignLeft to="/admin">Admin</Button>
              </>
          )}
      </aside>
  )
}

export default React.forwardRef(Menu)