function archive( )
{
   ARCHIVE_NAME=${PROJECT_CONFIG[ROOT_DIR_NAME]}_$(date +'%Y-%m-%d_%H-%M-%S')
   print_info "archiving project to " ${ARCHIVE_NAME}
   zip -r ../${ARCHIVE_NAME} ../${PROJECT_CONFIG[ROOT_DIR_NAME]}

   local RESULT=$?
   if [ 0 -eq ${RESULT} ]; then
      print_ok "archiving result: " ${RESULT}
   else
      print_error "archiving result: " ${RESULT}
      exit ${RESULT}
   fi
   return ${RESULT}
}
