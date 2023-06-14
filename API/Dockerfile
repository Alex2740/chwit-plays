FROM python:3.9

COPY ./main.py /code/main.py
COPY ./requirements.txt /code/requirements.txt
COPY ./.env ./code/.env

RUN python3 -m venv env
ENV PATH="/env/bin:$PATH"

RUN pip install -r /code/requirements.txt
WORKDIR /code

CMD [ "uvicorn", "main:app", "--host=0.0.0.0", "--port=4000" ]