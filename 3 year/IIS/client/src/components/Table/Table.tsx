import React, { useMemo } from 'react'
import classes from "./Table.module.css"
import classNames from 'classnames'
import { status } from '../../utils/common'
import uuid from 'react-uuid';

export type TableHeaderType = {
    [k: string]: string
}

type TableDataType = {
    [k: keyof TableHeaderType]: any
}[]

type PropsType = {
    dataKeys: TableHeaderType,
    data: TableDataType,
    actions?: React.ReactNode
    fmt?: (key: string, value: any) => React.ReactNode
    rowActions?: (value: any) => React.ReactNode
}

export const fmt = (v: any): string => {
    return [null, undefined, ""].includes(v) ? "--" : String(v)
}

const specFmt = (v: string) => {

    const statusClass = {
        [status.ACCEPTED]: classes.accepted,
        [status.PENDING]: classes.pending,
        [status.REJECTED]: classes.rejected,
    }[v]

    const cellStyles = classNames(classes.status, statusClass)

    if (statusClass) {
        return <span className={cellStyles}>{v}</span>
    }

    return v
}

/**
 * 
 * @param param0 
 * @returns 
 */
const Table = ({
    dataKeys,
    data,
    actions = null,
    fmt: _fmt,
    rowActions
}: PropsType) => {

    const keys = useMemo(
        () => Object.keys(dataKeys),
        [dataKeys]
    )

    const titles = useMemo(
        () => Object.values(dataKeys),
        [dataKeys]
    )

    const headStyles = classNames(classes.cell, classes.head)

    const header = titles.map(k => (
        <div key={uuid()} className={headStyles}>{k}</div>
    ))
    const body = data.map((item, i) => {

        const cellStyles = classNames(classes.cell, { [classes.zebra]: i % 2 })

        return (
            [
                ...keys.map(k => (
                    <div key={uuid()} className={cellStyles}>
                        {_fmt ? specFmt(_fmt(k, item[k]) as string) : specFmt(fmt(item[k]))}
                    </div>
                )),
                ...(rowActions ? [<div key={uuid()} className={classNames(cellStyles, classes.actions)}>{rowActions(item)}</div>] : [])
            ]
        )
    })

    return (
        <>
            <div className={classes.filters}>
                <div className={classes.innerFilters}>
                    {/* <Input className={classes.search} value={''} /> */}
                </div>
                {actions && (
                    <div className={classes.actions}>
                        {actions}
                    </div>
                )}
            </div>
            <div className={classes.container} style={{ gridTemplateColumns: `repeat(${keys.length}, auto) ${rowActions ? "min-content" : ""}` }}>
                {header}
                {rowActions && <div className={headStyles} />}
                {body}
            </div>
        </>
    )
}

export default Table