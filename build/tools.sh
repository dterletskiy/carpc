function archive( )
{
   ARCHIVE_NAME=${PROJECT_CONFIG[ROOT_DIR_NAME]}_$(date +'%Y-%m-%d_%H-%M-%S')
   zip -r ../${ARCHIVE_NAME} ../${PROJECT_CONFIG[ROOT_DIR_NAME]}
   echo ${ARCHIVE_NAME}
}
