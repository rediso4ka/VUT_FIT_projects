/**
 * @fileoverview Modal component implementation
 *
 * This file contains implementation of a Modal window component.
 *
 * @module Modal
 * 
 * @author xturyt00
 */
import React, { MouseEventHandler, useContext } from 'react'
import classes from "./Modal.module.css"
import { FloatingPortal } from '@floating-ui/react'
import { AppContext, floatingRoot } from '../../context/AppContextProvider'
import { Icon } from '@iconify/react'
import icons from '../../utils/icons'
// import Button from '../Button/Button'
import classNames from 'classnames'
import Button from '../Button/Button'
import StarRequire from '../StarRequire/StarRequire'

type PropsType = {
    title: string
    children: React.ReactNode
    textProceed: string
    textCancel: string
    onClose: () => void
    onSubmit: () => void
    error?: string
    icon: icons
    type?: ModalStyles
    disabled?: boolean
}

export enum ModalStyles {
    Inputs
}

/**
 * Prevents click event from bubbling up the DOM
 * 
 * @param e - Event
 */
const stopPropagation: MouseEventHandler<HTMLDivElement> = (e) => {
    e.stopPropagation()
}

/**
 * Modal window component
 * 
 * @param props - Component props
 * @param props.title - Title
 * @param props.children - Children
 * @param props.error - Error text
 * @param props.textProceed - Proceed text
 * @param props.textCancel - Cancel text
 * @param props.icon - Icon
 * @param props.onClose - Callback to close the modal window
 * @param props.onSubmit - Callback to proceed the modal window
 * @returns Modal window component
 */
const Modal = ({
    title,
    children,
    error = "",
    textProceed,
    textCancel,
    icon,
    onClose,
    onSubmit,
    type,
    disabled
}: PropsType) => {

    // const { isDark } = useContext(AppContext)

    const headerStyles = classNames(classes.header)

    const containerStyles = classNames(classes.container)

    const closeStyles = classNames(classes.close)

    const contentStyles = classNames(classes.content, {
        [classes.contentInputs]: type === ModalStyles.Inputs
    })

    return (
        <FloatingPortal root={floatingRoot}>
            <div className={classes.outer} onClick={onClose}>
                <div className={containerStyles} onClick={stopPropagation}>
                    <header className={headerStyles}>
                        <div className={classes.headerTitle}>
                            <Icon icon={icon} height={25} width={25} />
                            {title}
                        </div>
                        <button className={closeStyles} onClick={onClose}>
                            <Icon icon={icons.close} height={25} width={25} />
                        </button>
                    </header>
                    <div className={contentStyles}>
                        {/* {type === ModalStyles.Inputs && (
                            <div className={classes.requireText}>Fields with <StarRequire /> are required</div>
                        )} */}
                        {children}
                    </div>
                    <footer className={classes.footer}>
                        <div className={classes.errorContainer}>
                            {error && (
                                <p className={classes.error}>
                                    <Icon icon={icons.error} height={20} width={20} /> {error}
                                </p>
                            )}
                        </div>
                        <div className={classes.actions}>
                            <Button onClick={onClose}>
                                {textCancel}
                            </Button>
                            <Button disabled={disabled} style='invert' onClick={onSubmit}>
                                {textProceed}
                            </Button>
                        </div>
                    </footer>
                </div>
            </div>
        </FloatingPortal>
    )
}

export default Modal