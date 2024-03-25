import React, { useContext, useEffect, useState } from 'react'
import { AppContext, LoadingType } from '../../../../context/AppContextProvider'
import Dropdown from '../../../../components/Dropdown/Dropdown'
import { SpringResponseType, status } from '../../../../utils/common'

import classes from "./Categories.module.css"
import Input from '../../../../components/Input/Input'
import InputLabel from '../../../../components/InputLabel/InputLabel'
import Table, { TableHeaderType } from '../../../../components/Table/Table'
import TableView from '../../../../components/TableView/TableView'
import Button from '../../../../components/Button/Button'
import CreateCategoryModal, { CategoryInput } from './modals/CreateCategoryModal/CreateCategoryModal'
import ButtonIconOnly from '../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../utils/icons'
import RowActions from './components/RowActions/RowActions'
import { Icon } from '@iconify/react'

export type CategoryType = {
    id: number,
    name: string,
    status: string,
    parentCategory: number | null
}

const dataKeys: TableHeaderType = {
    id: "Id",
    name: "Name",
    parentCategory: "Parent Id",
    status: "Status",
}

export const categoriesToDropdown = (categories: CategoryType[]) => {
    return categories.map(({ id, name }) => ({ id: id?.toString(), value: name }))
}

const Categories = () => {

    const context = useContext(AppContext)

    const [isCreateActive, setIsCreateActive] = useState(false)
    const [categories, setCategories] = useState<CategoryType[]>([])

    const onSubmit = async (inputs: CategoryInput) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.post("/category", {
                ...inputs
            })
            if (response.status === 200) {
                setCategories(prev => [...prev, { id: response.data.categoryId, ...inputs } as CategoryType])
                setIsCreateActive(false)
            }
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const fetchCategories = async () => {
        context.setLoading(LoadingType.FETCHING)
        try {
            const response = await context.request!.get("/categories")

            const responses = await Promise.allSettled(
                response.data.categories.map(async (id: number) => await context.request!.get(`/category/${id}`))
            );

            const fulfilledResponses = responses
                .filter((r): r is PromiseFulfilledResult<SpringResponseType<CategoryType>> => r.status === "fulfilled")
                .map((r) => r.value)
                .filter((v) => v);

            setCategories(fulfilledResponses.map(({ data }) => data))
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    useEffect(() => {
        fetchCategories()
    }, [])

    return (
        <TableView>
            <Table
                dataKeys={dataKeys}
                data={categories}
                rowActions={(category) => (
                    <RowActions
                        categories={categories}
                        setCategories={setCategories}
                        category={category}
                    />
                )}
                actions={
                    <>
                        <Button style='invert' onClick={() => setIsCreateActive(true)}>
                            <Icon icon={icons.plus} width={20} height={20} />
                            Create category
                        </Button>
                        {isCreateActive && (
                            <CreateCategoryModal
                                icon={icons.plus}
                                textProceed='Create'
                                title='Create category'
                                categories={categories}
                                onClose={() => setIsCreateActive(false)}
                                onSubmit={onSubmit}
                            />
                        )}
                    </>
                } />
        </TableView>
    )
}

export default Categories
