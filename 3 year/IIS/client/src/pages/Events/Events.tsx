import React, { useContext, useEffect, useState } from 'react'

import classes from "./Events.module.css"
import { AppContext, LoadingType } from '../../context/AppContextProvider'
import PageView from '../../components/PageView/PageView'
import { EventType } from '../../utils/types'
import EventCard, { EventTypeFull } from './components/EventCard/EventCard'
import Button from '../../components/Button/Button'
import Input from '../../components/Input/Input'
import InputLabel from '../../components/InputLabel/InputLabel'
import Dropdown from '../../components/Dropdown/Dropdown'
import { CategoryType, categoriesToDropdown } from '../Admin/pages/Categories/Categories'

const Events = () => {
  const context = useContext(AppContext)

  const [search, setSearch] = useState("")
  const [events, setEvents] = useState<EventTypeFull[]>([])
  const [category, setCategory] = useState<number | null>(null)

  const [categories, setCategories] = useState<CategoryType[]>([])

  const fetchEvents = async () => {
    context.setLoading(LoadingType.FETCHING)
    try {
      const response = await context.request!.get("/events")

      const responses = await Promise.allSettled(
        response.data.events.map(async (id: number) => await context.request!.get(`/event/${id}`))
      );

      const fulfilledResponses = responses
        .filter((r): r is PromiseFulfilledResult<any> => r.status === "fulfilled")
        .map((r) => r.value)
        .filter((v) => v);
      
      const allCategoriesResponse = await context.request!.get("/categories")

      const allCategoriesResponses = await Promise.allSettled(
        allCategoriesResponse.data.categories.map(async (id: number) => await context.request!.get(`/category/${id}`))
      );

      const fulfilledAllCategoriesResponses = allCategoriesResponses
        .filter((r): r is PromiseFulfilledResult<any> => r.status === "fulfilled")
        .map((r) => r.value)
        .filter((v) => v);
      
      setCategories(fulfilledAllCategoriesResponses.map(({data}) => data))

      const authorIds = Array.from(new Set(fulfilledResponses.map(({ data }) => data.authorId)))
      const placeIds = Array.from(new Set(fulfilledResponses.map(({ data }) => data.placeId)))
      const categoryIds = Array.from(new Set(fulfilledResponses.map(({ data }) => data.categoryId)))

      const authorResponses = await Promise.allSettled(
        authorIds.map(async (id: number) => await context.request!.get(`/user/${id}`))
      );

      const fulfilledAuthorResponses = authorResponses
        .filter((r): r is PromiseFulfilledResult<any> => r.status === "fulfilled")
        .map((r) => r.value)
        .filter((v) => v);

      const placeResponses = await Promise.allSettled(
        placeIds.map(async (id: number) => await context.request!.get(`/place/${id}`))
      );

      const fulfilledPlaceResponses = placeResponses
        .filter((r): r is PromiseFulfilledResult<any> => r.status === "fulfilled")
        .map((r) => r.value)
        .filter((v) => v);
      
      const categoryResponses = await Promise.allSettled(
        categoryIds.map(async (id: number) => await context.request!.get(`/category/${id}`))
      );

      const fulfilledCategoryResponses = categoryResponses
        .filter((r): r is PromiseFulfilledResult<any> => r.status === "fulfilled")
        .map((r) => r.value)
        .filter((v) => v);

      const authors = fulfilledAuthorResponses.map(({ data }) => data)
      const places = fulfilledPlaceResponses.map(({ data }) => data)
      const categories = fulfilledCategoryResponses.map(({ data }) => data)

      setEvents(fulfilledResponses.map(({ data }) => {
        return {
          ...data,
          authorId: authors.find(({ id }) => id === data.authorId),
          categoryId: categories.find(({ id }) => id === data.categoryId),
          placeId: places.find(({ id }) => id === data.placeId)
        }
      }))

    } catch (error) {
      console.error(error)
    } finally {
      context.setLoading(LoadingType.NONE)
    }
  }

  const searchEvents: React.ChangeEventHandler<HTMLInputElement> = (e) => {
    setSearch(e.target.value)
  }

  useEffect(() => {
    fetchEvents()
  }, [])

  const hasCategory = (c: CategoryType | null): boolean => {
    if (c === null) return false
    
    return hasCategory(categories.find(({ id }) => c.parentCategory === id) ?? null) || c.id === category
  }

  const filteredEvents = events.filter(({ title, categoryId }) => (
      title.trim().toLowerCase().includes(search.trim().toLowerCase()) &&
      (category ? hasCategory(categoryId) : true)
  ))

  return (
    <PageView scroll title='Events'>
      <div className={classes.filtersContainer}>
        <div className={classes.filters}>
          <Input placeholder='Search...' className={classes.search} value={search} onChange={searchEvents} />
          <div className={classes.filterItem}>
            <InputLabel value='Category'>
              <Dropdown
                name='category'
                value={String(category)}
                items={categoriesToDropdown(categories)}
                onChange={(value) => setCategory(+value)}
              />
            </InputLabel>
          </div>
        </div>
        <div className={classes.actions}>
          {context.isAuth && <Button style='invert' to="/events/create">Create new event</Button>}
        </div>
      </div>
      <div className={classes.events}>
        {filteredEvents.map(event => <EventCard key={event.id} event={event} />)}
      </div>
    </PageView>
  )
}

export default Events