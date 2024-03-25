import React, { useContext, useEffect, useState } from 'react'
import { EventType } from '../../../../utils/types'

import classes from "./EventCard.module.css"
import { Icon } from '@iconify/react'
import icons from '../../../../utils/icons'
import Button from '../../../../components/Button/Button'
import { Link } from 'react-router-dom'
import ProfileCard from '../../../../components/ProfileCard/ProfileCard'
import { AppContext, UserType, timeAgo } from '../../../../context/AppContextProvider'
import { formatDate } from '../../../../utils/common'
import { PlaceType } from '../../../Admin/pages/Places/Places'
import { CategoryType } from '../../../Admin/pages/Categories/Categories'
import Popover from '../../../../components/Popover/Popover'

export type EventTypeFull = EventType & {
    authorId: UserType,
    placeId: PlaceType,
    categoryId: CategoryType
}

type PropsType = {
    event: EventTypeFull
}

const EventCard = ({
    event
}: PropsType) => {

    const context = useContext(AppContext)

    // const [author, setAuthor] = useState<UserType | null>(null)

    // const fetchAuthor = async () => {
    //     try {
    //         const response = await context.request!.get(`/user/${event.authorId}`)
    //         setAuthor(response.data)
    //     } catch (error) {
    //         console.log(error)
    //     }
    // }

    // useEffect(() => {
    //     fetchAuthor()
    // }, [])

    return (
        <div className={classes.container}>
            <div className={classes.preview} style={{ backgroundImage: `url(${event.image})` }} >
                {event.authorId && <ProfileCard className={classes.profile} user={event.authorId} />}
                <span className={classes.dateFrom}>{timeAgo.format(new Date(event.dateFrom))}</span>
            </div>
            <div className={classes.header}>
                <h2>{event.title}</h2>
            </div>
            <div className={classes.location}>
                {event.categoryId && (
                    <span className={classes.info}>
                        <Popover element={<Icon icon={icons.tag} width={20} height={20} />}>
                            Category
                        </Popover>
                        {event.categoryId.name}
                    </span>
                )}
            </div>
            <div className={classes.date}>
                <span className={classes.info}>
                    <Popover element={<Icon icon={icons.calendar} width={20} height={20} />}>
                        Event dates
                    </Popover>
                    {formatDate(event.dateFrom)} {event.dateTo && (
                        <>
                            - {formatDate(event.dateTo)}
                        </>
                    )}
                </span>
            </div>
            {event.placeId && (
                <div className={classes.location}>
                    <span className={classes.info}>
                        <Popover element={<Icon icon={icons.location} width={20} height={20} />}>
                            Location
                        </Popover>
                        {event.placeId.name}
                    </span>
                </div>
            )}
            <div className={classes.actions}>
                <Button style='invert' to={`/events/${event.id}`}>Details</Button>
            </div>
        </div>
    )
}

export default EventCard