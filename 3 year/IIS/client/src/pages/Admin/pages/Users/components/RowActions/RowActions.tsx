import React, { useContext, useState } from 'react'
import ButtonIconOnly from '../../../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../../../utils/icons'
import DeleteModal from '../../../../../../modals/DeleteModal/DeleteModal'
import { UserType } from '../../Users'
import { AppContext, LoadingType } from '../../../../../../context/AppContextProvider'
import Popover from '../../../../../../components/Popover/Popover'

type PropsType = {
    user: UserType
    users: UserType[]
    setUsers: React.Dispatch<React.SetStateAction<UserType[]>>
}

const RowActions = ({
    user,
    users,
    setUsers
}: PropsType) => {
    const context = useContext(AppContext)

    const [isDeleteActive, setIsDeleteActive] = useState(false)

    if (user.id === context.user.id || user.role === "ROLE_ADMIN") {
        return null;
    }

    const deleteUser = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.delete(`/user/${user.id}`)

            if (response.status === 200) {
                setUsers(prev => prev.filter(({ id }) => id !== user.id))
                setIsDeleteActive(false)
            }
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }
    return (
        <>
            <Popover element={
                <ButtonIconOnly icon={icons.trash} onClick={() => setIsDeleteActive(true)}></ButtonIconOnly>
            }>
                Delete
            </Popover>
            {isDeleteActive && (
                <DeleteModal
                    title={`Delete user "${user.login}"?`}
                    onSubmit={deleteUser}
                    onClose={() => setIsDeleteActive(false)}
                />
            )}
        </>
    )
}

export default RowActions