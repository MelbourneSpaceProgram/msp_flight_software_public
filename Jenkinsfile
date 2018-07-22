pipeline {

    agent none

    stages {
       stage('Static Analysis') {
            agent {
                label 'master'
            }
			environment {
				PR_ID = sh (
					script: 'if [ -z ${CHANGE_ID+x} ]; then echo "-1"; else echo "$CHANGE_ID"; fi',
					returnStdout: true
					).trim()
			}
            steps {
                sh 'if [ -d "checker_output" ]; then rm -Rf checker_output; fi'
                sh 'mkdir checker_output'
                sh 'python cpplint.py --recursive src 2>&1 | tee ./checker_output/cpplint.txt'
                // Check conditional branches for "#define A" only. This doesn't exist, which means we skip the poor-scaling of checking all possible
                // configurations, with the trade-off of weaker static analysis results.
                sh 'cppcheck -DA --enable=warning,performance,style,portability --inconclusive --xml --xml-version=2 -i"TIRTOS Build" -itest/ . 2> ./checker_output/cppcheck.xml'
                step([$class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false, defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', parserConfigurations: [[parserName: 'CppLint', pattern: '**/checker_output/cpplint.txt']], unHealthy: ''])
                sh 'cppcheck-htmlreport --source-encoding="iso8859-1" --title="MSP" --source-dir=. --report-dir=./checker_output/ --file=./checker_output/cppcheck.xml' 
                publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: './checker_output/', reportFiles: 'index.html', reportName: 'Static Analysis', reportTitles: ''])
                step([
                    $class: 'ViolationsToGitHubRecorder', 
                    config: [
                        gitHubUrl: 'https://api.github.com/', 
                        repositoryOwner: 'MelbourneSpaceProgram', 
                        repositoryName: 'flight_software', 
                        pullRequestId: "${PR_ID}", 
                        createCommentWithAllSingleFileComments: false, 
                        createSingleFileComments: true, 
                        commentOnlyChangedContent: false,       
                        credentialsId: 'a2c805e2-79f1-4a00-9fa5-d8e144e50245',
                        minSeverity: 'INFO',
                        keepOldComments: false,
                        violationConfigs: [
                            [ pattern: '.*/checker_output/.*\\.xml$', parser: 'CPPCHECK', reporter: 'CPPCHECK' ]
                        ]
                    ]
                ])
            }
        }
    
        stage('Build') { 
            agent {
                label 'AWS_Docker_Agent'
            }
			environment {
				container_uuid = "${BUILD_TAG}-PR${CHANGE_ID}"
				docker_name = "${GIT_COMMIT}_${BUILD_ID}_${container_uuid}"
			}
            steps {
                sh '''
                    tar cvf CDH_software.tar.gz -C ${WORKSPACE} .
                    sudo docker run -td --name ${docker_name} ccsv8_msp432e
                    sudo docker cp ${WORKSPACE}/CDH_software.tar.gz $docker_name:/root/
		    sudo docker exec -t $docker_name mkdir /root/flight_software
                    sudo docker exec -t $docker_name tar -xf /root/CDH_software.tar.gz -C /root/flight_software/
                    sudo docker exec -t $docker_name /opt/ti/ccsv8/eclipse/eclipse -noSplash -data /root/ws -application com.ti.ccstudio.apps.projectBuild -ccs.workspace -ccs.configuration "Tests MSP432E"
		    sudo docker cp $docker_name:"/root/flight_software/Tests MSP432E/MSP.out" ${WORKSPACE}/MSP.out 
		    '''
		    stash includes: 'MSP.out', name: 'flight_software_binary'
    	    }
            post {
                always {
		    warnings canComputeNew: false, canResolveRelativePaths: false, canRunOnFailed: true, categoriesPattern: '', consoleParsers: [[parserName: 'Texas Instruments Code Composer Studio (C/C++)']], defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', unHealthy: ''
                    sh '''
                      sudo docker rm -f ${docker_name}
                    '''
                }
            }
        }
    }
}
