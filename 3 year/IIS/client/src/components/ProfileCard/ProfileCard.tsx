import React from 'react'
import { Link } from 'react-router-dom'
import { UserType } from '../../context/AppContextProvider'

import classes from "./ProfileCard.module.css"
import classNames from 'classnames'

type PropsType = {
    user: UserType
    className?: string
}

const ProfileCard = ({
    user,
    className
}: PropsType) => {
    const containerStyles = classNames(classes.profile, className)
    return (
        <Link to={`/profile/${user?.id}`} className={containerStyles}>
          <div className={classes.ava}></div>
          <div className={classes.author}>
              <h3>{user?.email}</h3>
          </div>
      </Link>
  )
}

export default ProfileCard