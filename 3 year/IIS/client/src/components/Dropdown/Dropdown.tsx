/**
 * @fileoverview Dropdown implementation
 *
 * This file contains implementation of a Dropdown. This component is
 * inherited from DismissWindow and widen its implementation into Dropdown.
 *
 * @module Dropdown
 * 
 * @author xturyt00
 */
import { useContext } from 'react'
import DismissWindow from "../DismissWindow/DismissWindow"
import { placements } from '../../utils/common'
import { Icon } from '@iconify/react'
import icons from '../../utils/icons'
import { AppContext } from '../../context/AppContextProvider'
import classes from "./Dropdown.module.css"
import classNames from 'classnames'
import Input from '../Input/Input'

export type DropdownItemType = {
    id: string | null,
    value: string
}

type PropsType = {
    value: string | null
    items: DropdownItemType[]
    name?: string
    label?: string
    onChange: (value: string, name: string) => void
    actions?: React.ReactNode
}

const defaultItem = { id: null, value: "--" }

/**
 * Dropdown component
 * 
 * @param props - Component props
 * @param props.label - Label text
 * @param props.value - Value
 * @param props.items - Dropdown items
 * @param props.name - Name
 * @param props.onChange - Callback to change the button
 * @returns Dropdown component
 */
const Dropdown = ({
    label = "",
    value,
    items: defaultItems,
    name = "",
    onChange,
    actions
}: PropsType) => {

    // const { isDark } = useContext(AppContext)
    const currentValue = defaultItems.find(({ id }) => String(id) === String(value))?.value ?? defaultItem.value

    const items = [defaultItem, ...defaultItems]

    const dropdownStyles = classNames(classes.dropdown)

    const itemStyles = classNames(classes.item)

    return (
        <div className={dropdownStyles}>
            {label && (
                <span className={classes.label}>
                    {label}
                </span>
            )}
            <div className={classes.dropdownInputContainer}>

                <DismissWindow
                    align
                    offset={0}
                    placement={placements.BOTTOM}
                    element={(isActive) =>


                        <div className={classes.innerInputContainer}>
                            <div className={classes.dropdownInput} >{currentValue}</div>
                            <Icon className={classes.icon} icon={isActive ? icons.arrowUp : icons.arrowDown} height={20} width={20} />
                        </div>

                    }>
                    {setIsActive =>
                        <div className={classes.outerContainer}>
                            <div className={classes.container}>
                                {items.map(({ id, value }) => (
                                    <button key={id} className={itemStyles} onClick={() => { setIsActive(false); onChange(id!, name) }}>
                                        {value}
                                    </button>
                                ))}
                            </div>
                        </div>
                    }
                </DismissWindow>
                {actions}
            </div>
        </div>

    )
}

export default Dropdown