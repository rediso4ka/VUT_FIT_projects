import React, { useContext, useState } from 'react'
import { AppContext, LoadingType } from '../../../../../../context/AppContextProvider'
import { PlaceType } from '../../Places'
import ButtonIconOnly from '../../../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../../../utils/icons'
import DeleteModal from '../../../../../../modals/DeleteModal/DeleteModal'
import CreateCategoryModal from '../../../Categories/modals/CreateCategoryModal/CreateCategoryModal'
import CreatePlaceModal from '../../modals/CreatePlaceModal/CreatePlaceModal'
import Popover from '../../../../../../components/Popover/Popover'
import { status } from '../../../../../../utils/common'

type PropsType = {
    place: PlaceType
    places: PlaceType[]
    setPlaces: React.Dispatch<React.SetStateAction<PlaceType[]>>
}

const RowActions = ({
    place,
    places,
    setPlaces
}: PropsType) => {

    const context = useContext(AppContext)

    const [isUpdateActive, setIsUpdateActive] = useState(false)
    const [isDeleteActive, setIsDeleteActive] = useState(false)

    const updatePlace = async (inputs: PlaceType) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const newCategory = {
                ...inputs
            }
            const response = await context.request!.patch(`/place/${place.id}`, newCategory)

            setPlaces(prev => prev.reduce((a, c, i) => [...a, c.id === place.id ? newCategory : c], [] as PlaceType[]))
            setIsUpdateActive(false)
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const deletePlace = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.delete(`/place/${place.id}`)

            setPlaces(prev => prev.filter(({ id }) => id !== place.id))
            setIsDeleteActive(false)
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const acceptPlace = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.patch(`/place/${place.id}`, {
                ...place,
                status: status.ACCEPTED
            })

            setPlaces(prev => prev.reduce((a, p) => [...a, p.id === place.id ? { ...place, status: status.ACCEPTED } : p], [] as PlaceType[]))
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    return (
        <>
            {place.status === status.PENDING && (
                <Popover element={<ButtonIconOnly icon={icons.check} onClick={acceptPlace}></ButtonIconOnly>}>
                    Accept
                </Popover>
            )}
            <Popover element={<ButtonIconOnly icon={icons.pen} onClick={() => setIsUpdateActive(true)}></ButtonIconOnly>}>
                Update
            </Popover>
            <Popover element={<ButtonIconOnly icon={icons.trash} onClick={() => setIsDeleteActive(true)}></ButtonIconOnly>}>
                Delete
            </Popover>
            {isDeleteActive && (
                <DeleteModal
                    title='Delete place?'
                    onSubmit={deletePlace}
                    onClose={() => setIsDeleteActive(false)}
                />
            )}
            {isUpdateActive && (
                <CreatePlaceModal
                    icon={icons.pen}
                    title='Update place'
                    textProceed='Update'
                    inputs={place}
                    places={places}
                    onSubmit={updatePlace}
                    onClose={() => setIsUpdateActive(false)}
                />
            )}
        </>
    )
}

export default RowActions