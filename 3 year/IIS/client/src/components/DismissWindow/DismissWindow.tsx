/**
 * @fileoverview DismissWindow component implementation
 *
 * This file contains implementation of a DismissWindow. This is
 * a floating component that will be generated in different root element.
 * Primary use of this component in application is for Dropdown and Modal window
 *
 * @module DismissWindow
 * 
 * @author xturyt00
 */

import React, { useState, cloneElement, useRef, useMemo, useEffect, ReactElement, JSXElementConstructor } from "react"
import { floatingRoot } from "../../context/AppContextProvider"
import { placements } from "../../utils/common"

import {
    FloatingPortal,
    useInteractions,
    useFloating,
    arrow,
    flip,
    shift,
    autoUpdate,
    offset,
    useDismiss,
    Boundary,
    size,
    hide,
    useClick,
} from "@floating-ui/react"

import classNames from "classnames"
import classes from './DismissWindow.module.css'

type PropsType = {
    children: (setIsActive: React.Dispatch<React.SetStateAction<boolean>>) => React.ReactNode
    element: (isActive: boolean) => ReactElement<any, string | JSXElementConstructor<any>>
    className?: string
    placement?: placements
    offset?: number
    align?: boolean
    enableArrows?: boolean
    dismissOnClick?: boolean
    boundary?: string | null
    disabled?: boolean
}

/**
 * DismissWindow component
 * 
 * @see https://floating-ui.com/ floating ui documentation
 * 
 * @param props - Component props
 * @param props.className - Classname
 * @param props.placement - Modal window placement
 * @param props.element - Element that toggles modal window
 * @param props.align - Align window width with element (default = false)
 * @param props.enableArrows - Enable arrow (default = false)
 * @param props.dismissOnClick - Dismiss on click (default = false)
 * @param props.boundary - Outer boundaries (default = document)
 * @param props.offset - Offset from element (default = 10)
 * @param props.disabled - Disable window (default = false)
 * @returns DismissWindow component
 * 
 * ```tsx
 * 
 * return (
 *  <div>
 *      <DismissWindow element = {<p>Element with hover</p>}>
 *          Modal window content here
 *      <DismissWindow/>
 *  </div>
 * )
 * ```
 */
const DismissWindow = ({
    children,
    element: Element,
    className = "",
    placement = placements.TOP,
    align = false,
    enableArrows = false,
    dismissOnClick = false,
    boundary = null,
    offset: defaultOffset = 10,
    disabled = false
}: PropsType) => {

    const [isActive, setIsActive] = useState<boolean>(false)
    const arrowRef = useRef<HTMLDivElement>(null)

    const {
        x, // x position of the floating element
        y, // y position of the floating element
        refs, // DismissWindow refs
        strategy, // position type (relative | absolute)
        context, // context
        placement: floatingPlacement,  // actual placement
        middlewareData,
    } = useFloating({
        placement, // placement of the DismissWindow relatively to its parent
        open: isActive, // state
        onOpenChange: setIsActive,
        middleware: [
            // flip placements
            flip({ 
                boundary: boundary === null ? document.body as Boundary : document.querySelector(boundary) as Boundary,
                fallbackPlacements: ["top"],
             }),
            // shift placements
            shift(),
            // arrow set up
            arrow({ element: arrowRef }),
            size({
                apply({ rects, elements }) {
                    if (align) {
                        elements.floating.style.width = rects.reference.width + "px"
                    }
                },
            }),
            hide(),
            // offset set up
            offset(defaultOffset),
        ],
        // auto update for scrolling and resizing
        whileElementsMounted: (reference, floating, update) =>
            autoUpdate(reference, floating, update, { elementResize: true, ancestorScroll: true }),
    })

    // Hover set up for the floating element
    const { getReferenceProps, getFloatingProps } = useInteractions([
        useClick(context, { enabled: !disabled }),
        useDismiss(context),
    ])

    // Closes dropdown on scroll when it's hidden for the user.
    useEffect(() => {
        if (middlewareData?.hide?.referenceHidden) {
            setIsActive(false)
        }
    }, [middlewareData])

    // Calculates arrow style
    const arrowStyles = useMemo(
        () => classNames({
            [classes.popoverArrowTop]: floatingPlacement.split("-")[0] === placements.BOTTOM,
            [classes.popoverArrowRight]: floatingPlacement.split("-")[0] === placements.LEFT,
            [classes.popoverArrowBottom]: floatingPlacement.split("-")[0] === placements.TOP,
            [classes.popoverArrowLeft]: floatingPlacement.split("-")[0] === placements.RIGHT,
        }),
        [floatingPlacement]
    )

    return (
        <>
            {cloneElement(Element(isActive), { ref: refs.setReference, ...getReferenceProps() })}
            <FloatingPortal root={boundary === null ? floatingRoot : document.querySelector(boundary) as HTMLElement}>
                {isActive && cloneElement(
                    children(setIsActive) as ReactElement<any, string | JSXElementConstructor<any>>,
                    {
                        ref: refs.setFloating,
                        style: {
                            position: strategy,
                            top: y ?? 0,
                            left: x ?? 0,
                        },
                        ...getFloatingProps(),
                        ...(dismissOnClick ? getReferenceProps() : {}),
                    },
                    <>
                        {enableArrows && (
                            <div
                                className={arrowStyles}
                                ref={arrowRef}
                                style={{
                                    position: "absolute",
                                    left: `${middlewareData?.arrow?.x}px`,
                                    top: `${middlewareData?.arrow?.y}px`,
                                    borderTopColor: "inherit",
                                    borderBottomColor: "inherit",
                                }}
                            ></div>
                        )}
                        {(children(setIsActive) as ReactElement<any, string | JSXElementConstructor<any>>).props.children}
                    </>
                )}
            </FloatingPortal>
        </>
    )
}

export default DismissWindow