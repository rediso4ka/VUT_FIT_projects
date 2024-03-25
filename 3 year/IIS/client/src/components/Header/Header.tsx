import React, { useContext, useState } from 'react'
import { Link } from 'react-router-dom'

import classes from "./Header.module.css"
import { AppContext } from '../../context/AppContextProvider'
import DismissWindow from '../DismissWindow/DismissWindow'
import { placements, roles } from '../../utils/common'

import { Icon } from "@iconify/react"
import icons from '../../utils/icons'
import Menu from '../Menu/Menu'
import { containerId } from '../Container/Container'
import Content from '../Content/Content'
import ButtonIconOnly from '../ButtonIconOnly/ButtonIconOnly'
import Button from '../Button/Button'

const Header = () => {
    // const [isMenuActive, setIsMenuActive] = useState(false)
    const context = useContext(AppContext)

    const logout = () => {
        context.logout()
    }

    // const toggleMenu = () => {
    //     setIsMenuActive(prev => !prev)
    // }

    return (
        <div className={classes.container}>
            <Content className={classes.content}>
                <div className={classes.logo}>
                    <DismissWindow
                        dismissOnClick
                        boundary={"#" + containerId}
                        element={(isActive) => (
                            <ButtonIconOnly icon={isActive ? icons.close : icons.menu} />
                        )}>
                        {(setIsActive) => (
                            <Menu setIsActive={setIsActive} />
                        )}
                    </DismissWindow>
                    <Link to="/">Events & Actions</Link>
                </div>

                <div className={classes.actions}>
                    {(context.isAuth && context.user) ?
                        (
                            <DismissWindow
                                dismissOnClick
                                placement={placements.BOTTOM}
                                offset={0}
                                align
                                element={
                                    (isActive) => (
                                        <div className={classes.profile}>
                                            <span>Hi, {context.user.login}</span>
                                            <div className={classes.ava} />
                                            <Icon icon={isActive ? icons.arrowUp : icons.arrowDown} height={20} width={20} />
                                        </div>
                                    )
                                }>
                                {(setIsActive) => (
                                    <div className={classes.profileMenu}>
                                        <Button alignLeft to={`/profile/${context.user.id}`}>Profile</Button>
                                        <Button alignLeft onClick={logout}>Sign out</Button>
                                    </div>
                                )}
                            </DismissWindow>
                        ) :
                        (
                            <>
                                <Button to='/login'>Login</Button>
                                <Button style='invert' to='/register/user'>Register</Button>
                            </>
                        )
                    }
                </div>
            </Content>
        </div>
    )
}

export default Header