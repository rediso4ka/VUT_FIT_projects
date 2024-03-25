import React, { MouseEventHandler } from 'react'
import Button from '../Button/Button'
import icons from '../../utils/icons'
import { Icon } from '@iconify/react'
import classNames from 'classnames'

import classes from './ButtonIconOnly.module.css'

type PropsType = {
    icon: icons
    onClick?: MouseEventHandler<HTMLButtonElement>
    className?: string,
    iconWidth?: number,
    iconHeight?: number,
    to?: string
}

const ButtonIconOnly = ({
    icon,
    className = "",
    onClick,
    iconWidth = 20,
    iconHeight = 20,
    to
}: PropsType, ref: React.Ref<HTMLButtonElement>) => {
  return (
      <Button ref={ref} to={to} className={classNames(className, classes.button)} onClick={onClick}>
          <Icon icon={icon} width={iconWidth} height={iconHeight} />
      </Button>
  )
}

export default React.forwardRef(ButtonIconOnly)