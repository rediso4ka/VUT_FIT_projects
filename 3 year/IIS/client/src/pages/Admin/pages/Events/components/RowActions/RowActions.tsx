import React, { useContext, useState } from 'react'
import { EventType } from '../../../../../../utils/types'
import { status } from '../../../../../../utils/common'
import Popover from '../../../../../../components/Popover/Popover'
import ButtonIconOnly from '../../../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../../../utils/icons'
import { AppContext, LoadingType } from '../../../../../../context/AppContextProvider'
import EventUpdateModal from '../../../../../EventDetail/modals/EventUpdateModal/EventUpdateModal'
import EventDeleteModal from '../../../../../EventDetail/modals/EventDeleteModal/EventDeleteModal'

type PropsType = {
    events: EventType[]
    event: EventType
    setEvents: React.Dispatch<React.SetStateAction<EventType[]>>
}

const RowActions = ({
    events,
    setEvents,
    event,
}: PropsType) => {

    const context = useContext(AppContext)

    const [isUpdateActive, setIsUpdateActive] = useState(false)
    const [isDeleteActive, setIsDeleteActive] = useState(false)

    const updateEvent = async (inputs: EventType) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const newEvent = {
                ...event,
                ...inputs
            }
            const response = await context.request!.patch(`/event/${event.id}`, newEvent)

            setEvents(prev => prev.reduce((a, e, i) => [...a, e.id === event.id ? newEvent : e], [] as EventType[]))
            setIsUpdateActive(false)
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const deleteEvent = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.delete(`/event/${event.id}`)

            setEvents(prev => prev.filter(({ id }) => id !== event.id))
            setIsDeleteActive(false)
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const acceptEvent = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.patch(`/event/${event.id}`, {
                ...event,
                status: status.ACCEPTED
            })

            setEvents(prev => prev.reduce((a, e) => [...a, e.id === event.id ? { ...event, status: status.ACCEPTED } : e], [] as EventType[]))
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    return (
        <>
            {event.status === status.PENDING && (
                <Popover element={<ButtonIconOnly icon={icons.check} onClick={acceptEvent}></ButtonIconOnly>}>
                    Accept
                </Popover>
            )}
            <Popover element={<ButtonIconOnly icon={icons.pen} onClick={() => setIsUpdateActive(true)}></ButtonIconOnly>}>
                Update
            </Popover>
            <Popover element={<ButtonIconOnly icon={icons.trash} onClick={() => setIsDeleteActive(true)}></ButtonIconOnly>}>
                Delete
            </Popover>
            {isUpdateActive && (
                <EventUpdateModal event={event} onClose={() => setIsUpdateActive(false)} onSubmit={updateEvent} />
            )}
            {isDeleteActive && (
                <EventDeleteModal onClose={() => setIsDeleteActive(false)} onSubmit={deleteEvent} />
            )}
        </>
    )
}

export default RowActions