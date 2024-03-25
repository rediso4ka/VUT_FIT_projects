import React, { useContext, useState } from 'react'
import { CategoryType } from '../../Categories'
import ButtonIconOnly from '../../../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../../../utils/icons'
import CreateCategoryModal, { CategoryInput } from '../../modals/CreateCategoryModal/CreateCategoryModal'
import { AppContext, LoadingType } from '../../../../../../context/AppContextProvider'
import DeleteModal from '../../../../../../modals/DeleteModal/DeleteModal'
import Popover from '../../../../../../components/Popover/Popover'
import { status } from '../../../../../../utils/common'

type PropsType = {
    category: CategoryType
    categories: CategoryType[]
    setCategories: React.Dispatch<React.SetStateAction<CategoryType[]>>
}

const RowActions = ({
    category,
    categories,
    setCategories
}: PropsType) => {

    const context = useContext(AppContext)

    const [isUpdateActive, setIsUpdateActive] = useState(false)
    const [isDeleteActive, setIsDeleteActive] = useState(false)

    const updateCategory = async (inputs: CategoryInput) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const newCategory = {
                ...category,
                ...inputs
            }
            const response = await context.request!.patch(`/category/${category.id}`, newCategory)

            setCategories(prev => prev.reduce((a, c, i) => [...a, c.id === category.id ? newCategory : c], [] as CategoryType[]) )
            setIsUpdateActive(false)
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const deleteCategory = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.delete(`/category/${category.id}`)

            setCategories(prev => prev.filter(({ id, parentCategory }) => id !== category.id && parentCategory !== category.id))
            setIsDeleteActive(false)
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    const acceptCategory = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.patch(`/category/${category.id}`, {
                ...category,
                status: status.ACCEPTED
            })

            setCategories(prev => prev.reduce((a, c) => [...a, c.id === category.id ? { ...category, status: status.ACCEPTED } : c], [] as CategoryType[]))
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }
    
  return (
      <>
          {category.status === status.PENDING && (
              <Popover element={<ButtonIconOnly icon={icons.check} onClick={acceptCategory}></ButtonIconOnly>}>
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
                  title='Delete category?'
                  onSubmit={deleteCategory}
                  onClose={() => setIsDeleteActive(false)}
              />
          )}
          {isUpdateActive && (
              <CreateCategoryModal
                  icon={icons.pen}
                  title='Update category'
                  textProceed='Update'
                  inputs={category}
                  categories={categories}
                  onSubmit={updateCategory}
                  onClose={() => setIsUpdateActive(false)}
              />
          )}
      </>
  )
}

export default RowActions