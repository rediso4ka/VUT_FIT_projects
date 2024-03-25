import React, { MouseEventHandler } from 'react'
import classes from "./Button.module.css"
import classNames from 'classnames'
import { useNavigate } from 'react-router-dom'

type OnClickType = MouseEventHandler<HTMLButtonElement>

type PropsType = {
    to?: string
    onClick?: OnClickType
    children?: React.ReactNode
    className?: string
    style?: "invert"
    alignLeft?: boolean,
    disabled?: boolean
}

const Button = ({
    to,
    onClick = () => {},
    children,
    className,
    style,
    alignLeft = false,
    disabled
}: PropsType, ref: React.Ref<HTMLButtonElement>) => {
    const navigate = useNavigate()

    const __onClick: OnClickType = (event) => {
        if (to) {
            navigate(to)
        } else {
            onClick(event)
        }
    }

    const buttonStyles = classNames(className, classes.button, {
        [classes.invert]: style === "invert",
        [classes.alignLeft]: alignLeft
    })
  return (
      <button ref={ref} className={buttonStyles} onClick={__onClick} disabled={disabled}>{children}</button>
  )
}

export default React.forwardRef(Button)