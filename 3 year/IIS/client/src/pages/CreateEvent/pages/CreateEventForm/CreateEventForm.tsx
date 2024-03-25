import React, { useContext, useEffect, useState } from 'react'
import Input from '../../../../components/Input/Input'
import Textarea from '../../../../components/Textarea/Textarea'
import ReactDatePicker from 'react-datepicker'
import DateInput, { DateChangeType } from '../../../../components/DateInput/DateInput'
import TicketInputs from '../components/TicketInputs/TicketInputs'

import classes from "./CreateEventForm.module.css"
import { TicketType } from '../Tickets/modals/CreateTicketModal/CreateTicketModal'
import { AppContext, LoadingType } from '../../../../context/AppContextProvider'
import { ResponseMessageType, SpringResponseType } from '../../../../utils/common'
import { AxiosResponse } from 'axios'
import { PlaceType, placesToDropdown } from '../../../Admin/pages/Places/Places'
import { CategoryType, categoriesToDropdown } from '../../../Admin/pages/Categories/Categories'
import InputLabel from '../../../../components/InputLabel/InputLabel'
import Dropdown from '../../../../components/Dropdown/Dropdown'
import Button from '../../../../components/Button/Button'
import { TicketTypeWithRegister } from '../Tickets/Tickets'
import StarRequire from '../../../../components/StarRequire/StarRequire'
import { useNavigate } from 'react-router'
import ButtonIconOnly from '../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../utils/icons'
import CreateCategoryModal, { CategoryInput } from '../../../Admin/pages/Categories/modals/CreateCategoryModal/CreateCategoryModal'
import CreatePlaceModal from '../../../Admin/pages/Places/modals/CreatePlaceModal/CreatePlaceModal'
import Popover from '../../../../components/Popover/Popover'

import images from "./images.json"

const initialInputs = {
    title: "",
    description: "",
    dateFrom: "",
    dateTo: "",
    category: null,
    place: null
}

const isEmpty = (inputs: typeof initialInputs): boolean => {

    return (
        inputs.title.length === 0 ||
        inputs.dateFrom.length === 0 ||
        inputs.category === null ||
        inputs.place === null
    )
}

const isTicketsEmpty = (tickets: TicketTypeWithRegister[]): boolean => {
    if (tickets.length === 0) return true

    return tickets.some(ticket => (
        ticket.name.length === 0
    ))
}

const getRandomImage = () => {
    return images[Math.round(Math.random() * images.length)]
}

const CreateEventForm = () => {

    const navigate = useNavigate()

    const context = useContext(AppContext)

    const [inputs, setInputs] = useState(initialInputs)
    const [tickets, setTickets] = useState<TicketTypeWithRegister[]>([])
    const [places, setPlaces] = useState<PlaceType[]>([])
    const [categories, setCategories] = useState<CategoryType[]>([])

    const [newCategory, setNewCategory] = useState<CategoryType | null>(null)
    const [newPlace, setNewPlace] = useState<PlaceType | null>(null)

    const [isNewCategoryActive, setIsNewCategoryActive] = useState<boolean>(false)
    const [isNewPlaceActive, setIsNewPlaceActive] = useState<boolean>(false)

    const onChange = (e: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
        setInputs(prev => ({ ...prev, [e.target.name]: e.target.value }))
    }

    const onDateChange: DateChangeType = (name, value) => {

        setInputs(prev => ({ ...prev, [name]: value?.toISOString() }))
    }

    const onDropdownChange = (value: string | number, name: string) => {
        setInputs(prev => ({ ...prev, [name]: value }))
    }

    const fetchPlaces = async () => {
        const response = await context.request!.get("/places")

        const responses = await Promise.allSettled(
            response.data.places.map(async (id: number) => await context.request!.get(`/place/${id}`))
        );

        const fulfilledResponses = responses
            .filter((r): r is PromiseFulfilledResult<SpringResponseType<PlaceType>> => r.status === "fulfilled")
            .map((r) => r.value)
            .filter((v) => v);

        setPlaces(fulfilledResponses.map(({ data }) => data))
    }

    const fetchCategories = async () => {
        const response = await context.request!.get("/categories")

        const responses = await Promise.allSettled(
            response.data.categories.map(async (id: number) => await context.request!.get(`/category/${id}`))
        );

        const fulfilledResponses = responses
            .filter((r): r is PromiseFulfilledResult<SpringResponseType<CategoryType>> => r.status === "fulfilled")
            .map((r) => r.value)
            .filter((v) => v);

        setCategories(fulfilledResponses.map(({ data }) => data))
    }


    const fetch = async () => {
        context.setLoading(LoadingType.FETCHING)
        try {
            await fetchPlaces();
            await fetchCategories();
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const onSubmit = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.post("/event", {
                "title": inputs.title,
                "description": inputs.title,
                "dateFrom": inputs.dateFrom,
                "dateTo": inputs.dateTo,
                "categoryId": Number(inputs.category),
                "placeId": Number(inputs.place),
                "image": getRandomImage()
            })

            const eventId = response.data.eventId

            const responses = await Promise.allSettled(
                tickets.map(async ticket => await context.request!.post(`/event/${eventId}/ticket`, { ...ticket }))
            );
            
            navigate("/")
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const createCategory = async (inputs: CategoryInput) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.post("/category", {
                ...inputs
            })
            if (response.status === 200) {
                setCategories(prev => [...prev, { id: response.data.categoryId, ...inputs } as CategoryType])
                setIsNewCategoryActive(false)
            }
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const createPlace = async (inputs: PlaceType) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.post("/place", {
                ...inputs
            })
            if (response.status === 200) {
                setPlaces(prev => [...prev, { ...inputs, id: response.data.placeId }])
                setIsNewPlaceActive(false)
            }
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const createTicket = (inputs: TicketTypeWithRegister) => {
        setTickets(prev => [...prev, inputs])
    }

    const deleteTicket = (i: number) => () => {
        setTickets(prev => prev.filter((_, __i) => __i !== i))
    }

    const updateTicket = (i: number) => (input: TicketTypeWithRegister) => {
        setTickets(prev => prev.reduce((a, t, __i) => [...a, __i === i ? input : t], [] as TicketTypeWithRegister[]))
    }

    useEffect(() => {
        fetch()
    }, [])

    return (
        <div className={classes.container}>
            <h3 className={classes.sectionTitle}>General</h3>
            <Input required label='Title' name='title' value={inputs.title} onChange={onChange} />
            <InputLabel required value='Category'>
                <Dropdown
                    name='category'
                    value={inputs.category}
                    items={categoriesToDropdown(categories)}
                    onChange={onDropdownChange}
                    actions={
                        <Popover element={
                            <ButtonIconOnly icon={icons.plus} onClick={() => setIsNewCategoryActive(true)} />
                        }>
                            Create new category
                        </Popover>
                    }
                />
            </InputLabel>
            <DateInput required label='Start date' name='dateFrom' value={inputs.dateFrom} onChange={onDateChange} />
            <DateInput label='End date' name='dateTo' value={inputs.dateTo} onChange={onDateChange} />
            <InputLabel required value='Place'>
                <Dropdown
                    name='place'
                    value={inputs.place}
                    items={placesToDropdown(places)}
                    onChange={onDropdownChange}
                    actions={
                        <Popover element={
                            <ButtonIconOnly icon={icons.plus} onClick={() => setIsNewPlaceActive(true)} />
                        }>
                            Create new place
                        </Popover>
                    }
                />
            </InputLabel>
            <div className={classes.description}>

            <Textarea label='Description' name='description' value={inputs.description} onChange={onChange} />
            </div>
            <h3 className={classes.sectionTitle}>
                Tickets
                <StarRequire/>
            </h3>
            <div className={classes.tickets}>
                <TicketInputs
                    enableNewTicket
                    tickets={tickets}
                    createTicket={createTicket}
                    updateTicket={updateTicket}
                    deleteTicket={deleteTicket}
                />
            </div>
            <div className={classes.actions}>
                <Button onClick={() => navigate(-1)}>
                    Cancel
                </Button>
                <Button style='invert' disabled={isTicketsEmpty(tickets) || isEmpty(inputs)} onClick={onSubmit}>
                    Submit
                </Button>
            </div>
            {isNewCategoryActive && (
                <CreateCategoryModal
                    title='Create new category'
                    icon={icons.plus}
                    textProceed='Save'
                    categories={categories}
                    onSubmit={createCategory}
                    onClose={() => setIsNewCategoryActive(false)}
                />
            )}
            {isNewPlaceActive && (
                <CreatePlaceModal
                    icon={icons.plus}
                    places={places}
                    title="Create place"
                    onClose={() => setIsNewPlaceActive(false)}
                    onSubmit={createPlace}
                    textProceed='Save'
                />
            )}
        </div>
    )
}

export default CreateEventForm