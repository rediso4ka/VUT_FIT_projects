import React, { useContext, useState } from 'react'
import { CommentType } from '../../EventDetail'
import classes from "./Comment.module.css"
import ProfileCard from '../../../../components/ProfileCard/ProfileCard'
import Stars from '../Stars/Stars'
import { AppContext, LoadingType, timeAgo } from '../../../../context/AppContextProvider'
import Popover from '../../../../components/Popover/Popover'
import ButtonIconOnly from '../../../../components/ButtonIconOnly/ButtonIconOnly'
import icons from '../../../../utils/icons'
import { roles } from '../../../../utils/common'
import CommentUpdateModal from '../../modals/CommentUpdateModal/CommentUpdateModal'
import DeleteModal from '../../../../modals/DeleteModal/DeleteModal'

type PropsType = {
    comment: CommentType
    setComments: React.Dispatch<React.SetStateAction<CommentType[]>>
}

const Comment = ({
    comment,
    setComments
}: PropsType) => {
    const context = useContext(AppContext)

    const [isUpdateActive, setIsUpdateActive] = useState(false)
    const [isDeleteActive, setIsDeleteActive] = useState(false)

    const updateComment = async (inputs: CommentType) => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.patch(`/event/comment/${inputs.id}`, inputs)

            if (response.status === 200) {
                setComments(prev => prev.reduce((a, c, i) => [...a, c.id === inputs.id ? inputs : c], [] as CommentType[]))
                setIsUpdateActive(false)
            }
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }   

    const deleteComment = async () => {
        context.setLoading(LoadingType.LOADING)
        try {
            const response = await context.request!.delete(`/event/comment/${comment.id}`)

            if (response.status === 200) {
                setComments(prev => prev.filter(({ id }) => id !== comment.id))
                setIsDeleteActive(false)
            }
        } catch (error) {
            console.error(error)
        } finally {
            context.setLoading(LoadingType.NONE)
        }
    }

    return (
        <div className={classes.comment} key={comment.id}>
            <div className={classes.commentHeader}>
                <div className={classes.commentHeaderAuthor}>
                    <ProfileCard user={comment.user} />
                    <Stars rating={comment.rating} />
                    {(
                        context.isAuth && 
                        (
                            comment.user.id === context.user.id ||
                            context.user.role !== roles.USER
                        )
                    ) && (
                            <div className={classes.commentActions}>
                                <Popover element={<ButtonIconOnly icon={icons.pen} onClick={() => setIsUpdateActive(true)} />}>
                                    Update
                                </Popover>
                                <Popover element={<ButtonIconOnly icon={icons.trash} onClick={() => setIsDeleteActive(true)} />}>
                                    Delete
                                </Popover>
                            </div>
                        )}
                </div>

                <p className={classes.commentDate}>{timeAgo.format(new Date(comment.date))}</p>

            </div>
            <p>{comment.text}</p>
            {isUpdateActive && (
                <CommentUpdateModal
                    comment={comment}
                    onClose={() => setIsUpdateActive(false)}
                    onSubmit={updateComment}
                />
            )}
            {isDeleteActive && (
                <DeleteModal
                    title='Delete comment?'
                    onClose={() => setIsDeleteActive(false)}
                    onSubmit={deleteComment}
                />
            )}
        </div>
    )
}

export default Comment