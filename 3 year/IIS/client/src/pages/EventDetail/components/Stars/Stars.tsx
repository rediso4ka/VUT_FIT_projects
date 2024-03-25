import React from 'react'
import uuid from 'react-uuid';
import classes from "./Stars.module.css"

type PropsType = {
    setRating?: React.Dispatch<React.SetStateAction<number>>
    rating: number
}

const Stars = ({
    rating,
    setRating
}: PropsType) => {
  return (
      <div>
          {
              [...Array(5)].map((star, i) => {
                  const ratingValue = i + 1;
                  return (
                      <label key={uuid()} style={{ cursor: 'pointer' }}>
                          <input
                              type="radio"
                              value={ratingValue}
                              onClick={() => setRating ? setRating(ratingValue) : {}}
                              style={{ display: 'none' }}
                          />
                          <span className={ratingValue <= rating ? classes.filledStar : classes.emptyStar}
                              style={{ fontSize: '2em' }}
                          >
                              &#9733;
                          </span>
                      </label>
                  );
              }
              )}
      </div>
  )
}

export default Stars