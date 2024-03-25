import { UserType } from "../context/AppContextProvider"
import { CategoryType } from "../pages/Admin/pages/Categories/Categories"

export type EventType = {
    "id": number,
    "dateFrom": string,
    "dateTo": string,
    "description": string,
    "title": string,
    "icon": null,
    "image": null,
    "status": string,
    "placeId": number,
    "authorId": number,
    "categoryId": number
}