import {timeFormat} from 'd3'

export enum placements {
    TOP = "top",
    RIGHT = "right",
    BOTTOM = "bottom",
    LEFT = "left",
}

export enum roles {
    USER = "ROLE_USER",
    MANAGER = "ROLE_MANAGER",
    ADMIN = "ROLE_ADMIN"
}

export enum status {
    PENDING = "Pending",
    ACCEPTED = "Accepted",
    REJECTED = "Rejected"
}

export type SpringResponseType<T> = {
    data: T
}

export type ResponseMessageType = {
    message: string
    status: string
}

export const formatDate = (date: string | null) => {
    if (!date) return ""

    const format = timeFormat("%d.%m.%Y");
    
    return format(new Date(date))
}