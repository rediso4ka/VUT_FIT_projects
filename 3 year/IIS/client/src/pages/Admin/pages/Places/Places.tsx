import React, { useContext, useEffect, useState } from 'react'
import classes from "./Places.module.css"
import Input from '../../../../components/Input/Input'
import { AppContext, LoadingType } from '../../../../context/AppContextProvider'
import { SpringResponseType } from '../../../../utils/common'
import TableView from '../../../../components/TableView/TableView'
import Table, { TableHeaderType } from '../../../../components/Table/Table'
import Button from '../../../../components/Button/Button'
import CreatePlaceModal from './modals/CreatePlaceModal/CreatePlaceModal'
import RowActions from './components/RowActions/RowActions'
import { Icon } from '@iconify/react'
import icons from '../../../../utils/icons'

export type PlaceType = {
    id: number,
    name: string,
    description: string,
    address: string
    status: string
}


export const placesToDropdown = (places: PlaceType[]) => {
    return places.map(({ id, name }) => ({ id: id?.toString(), value: name }))
}

const dataKeys: TableHeaderType = {
    id: "Id",
    name: "Name",
    address: "Address",
    status: "Status"
}

const Places = () => {

    const context = useContext(AppContext)

    const [places, setPlaces] = useState<PlaceType[]>([])
    const [isCreateActive, setIsCreateActive] = useState(false)

    const fetchPlaces = async () => {
        context.setLoading(LoadingType.FETCHING)
        try {
            const response = await context.request!.get("/places")

            const responses = await Promise.allSettled(
                response.data.places.map(async (id: number) => await context.request!.get(`/place/${id}`))
            );

            const fulfilledResponses = responses
                .filter((r): r is PromiseFulfilledResult<SpringResponseType<PlaceType>> => r.status === "fulfilled")
                .map((r) => r.value)
                .filter((v) => v);

            setPlaces(fulfilledResponses.map(({ data }) => data))
        } catch (error) {
            console.log(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const onSubmit = async (inputs: PlaceType) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.post("/place", {
                ...inputs
            })
            setPlaces(prev => [...prev, { ...inputs, id: response.data.placeId }])
            setIsCreateActive(false)
        } catch (error) {
            console.log(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    useEffect(
        () => {
            fetchPlaces()
        },
        []
    )

    return (
        <TableView>
            <Table
                data={places}
                dataKeys={dataKeys}
                rowActions={(place) => (
                    <RowActions
                        places={places}
                        setPlaces={setPlaces}
                        place={place}
                    />
                )}
                actions={
                    <>
                        <Button style='invert' onClick={() => setIsCreateActive(true)}>
                            <Icon icon={icons.plus} width={20} height={20} />
                            Create place
                        </Button>
                        {isCreateActive && (
                            <CreatePlaceModal
                                icon={icons.plus}
                                textProceed='Create'
                                title='Create place'
                                places={places}
                                onClose={() => setIsCreateActive(false)}
                                onSubmit={onSubmit}
                            />
                        )}
                    </>
                }
            />
        </TableView>
    )
}

export default Places
