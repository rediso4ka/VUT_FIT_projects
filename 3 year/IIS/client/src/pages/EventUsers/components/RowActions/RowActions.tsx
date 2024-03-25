import React, { useContext, useState } from 'react'
import { UserWithStatus } from '../../EventUsers'
import ButtonIconOnly from '../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../utils/icons'
import { AppContext, LoadingType } from '../../../../context/AppContextProvider'
import { useParams } from 'react-router-dom'
import DeleteModal from '../../../../modals/DeleteModal/DeleteModal'
import { status } from '../../../../utils/common'
import Popover from '../../../../components/Popover/Popover'

type PropsType = {
    user: UserWithStatus,
    users: UserWithStatus[],
    setUsers: React.Dispatch<React.SetStateAction<UserWithStatus[]>>
}

const RowActions = ({
    user,
    users,
    setUsers
}: PropsType) => {
    const { id } = useParams()
    const context = useContext(AppContext)

    const [isDeleteActive, setIsDeleteActive] = useState(false)

    const checkUser = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            ///event/ticket/registration/{id}
            const response = await context.request!.patch(`/event/ticket/registration/${user.registerId}`, {
                userId: user.id,
                ticketId: user.ticketId,
                status: status.ACCEPTED,
                date: new Date()
            })
            setUsers(prev => prev.reduce((a, u) => [...a, u.id === user.id ? { ...user, status: status.ACCEPTED } : u ], [] as UserWithStatus[]))
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const deleteUser = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.patch(`/event/ticket/registration/${user.registerId}`, {
                userId: user.id,
                ticketId: user.ticketId,
                status: status.REJECTED,
                date: new Date()
            })
            setUsers(prev => prev.reduce((a, u) => [...a, u.id === user.id ? { ...user, status: status.REJECTED } : u], [] as UserWithStatus[]))
            setIsDeleteActive(false)
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }
    return (
        <>
            {(user.status === status.REJECTED || user.status === status.PENDING )&& (
                <Popover element={<ButtonIconOnly onClick={checkUser} icon={icons.check}></ButtonIconOnly>}>
                    Accept
                </Popover>
            )}
            {(user.status === status.ACCEPTED || user.status === status.PENDING) && (
                <Popover element={<ButtonIconOnly onClick={() => setIsDeleteActive(true)} icon={icons.close}></ButtonIconOnly>}>
                    Reject
                </Popover>
            )}
            {isDeleteActive && (
                <DeleteModal
                    title={`Dismiss user "${user.login}"?`}
                    onSubmit={deleteUser}
                    onClose={() => setIsDeleteActive(false)}
                />
            )}
        </>
    )
}

export default RowActions