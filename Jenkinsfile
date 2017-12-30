pipeline {
    
    agent any

    environment {
        // We need to create a separate folder for the CCS workspace
        // As it gets upset if we make it the same as our project directory
        CCS_WS_DIR = sh (
            script: 'mktemp -d',
            returnStdout: true
        ).trim()
    }

    post {
      failure {
        updateGitlabCommitStatus name: 'build', state: 'failed'
        addGitLabMRComment comment: ":new_moon_with_face: [Build failed!](${BUILD_URL}) :new_moon_with_face:\n\nPlease click the link and check Console Output to find out why."
      }
      success {
        updateGitlabCommitStatus name: 'build', state: 'success'
        addGitLabMRComment comment: ":full_moon_with_face: [Build succeeded!](${BUILD_URL}) :full_moon_with_face:\n\nPlease click the link and check for any warnings."
      }
    }
    
    options {
        gitLabConnection('MSP GitLab')
        // CCS will error if two projects try to build concurrently
        disableConcurrentBuilds()
    }

    triggers {
        gitlab(
            triggerOnPush: true,
            triggerOnMergeRequest: true,
            triggerOnNoteRequest: true,
            noteRegex: "Jenkins please retry a build",
            skipWorkInProgressMergeRequest: false,
            ciSkip: false,
            setBuildDescription: true,
            addNoteOnMergeRequest: true,
            addCiMessage: true,
            addVoteOnMergeRequest: true,
            acceptMergeRequestOnSuccess: false,
            branchFilterType: "All")
    }

    stages {
        stage('Checkout Source') {
            steps {
                checkout changelog: true, poll: true, scm: [
                    $class: 'GitSCM',
                    branches: [[name: "origin/${env.gitlabSourceBranch}"]],
                    doGenerateSubmoduleConfigurations: false,
                    extensions: [[$class: 'PreBuildMerge', options: [fastForwardMode: 'FF', mergeRemote: 'origin', mergeStrategy: 'default', mergeTarget: "${env.gitlabTargetBranch}"]]],
                    submoduleCfg: [],
                    userRemoteConfigs: [[name: 'origin', credentialsId: '5f8fc669-3371-4992-9c73-8ec204092c26', url: 'git@git.spaceprogram.melbourne:CS-1/CDH_software.git']]
                ]
            }
        }
        
        stage('Build') {
            steps {
                sh '/home/akremor/ti/ccsv7/eclipse/eclipse -noSplash -data "${CCS_WS_DIR}" -application com.ti.ccstudio.apps.projectImport -ccs.location $WORKSPACE'
                sh '/home/akremor/ti/ccsv7/eclipse/eclipse -noSplash -data "${CCS_WS_DIR}" -application com.ti.ccstudio.apps.projectBuild -ccs.workspace -ccs.configuration "TIRTOS Build"'
            }
        }
    stage('Warnings Report') {
        steps {
            warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'Texas Instruments Code Composer Studio (C/C++)']], defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', unHealthy: ''
            step([$class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false, defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', parserConfigurations: [[parserName: 'CppLint', pattern: '**/checker_output/cpplint.txt']], unHealthy: ''])
        }
    }
    stage('Static Analysis'){
            steps {
                sh 'if [ -d "checker_output" ]; then rm -Rf checker_output; fi'
                sh 'mkdir checker_output'
                sh 'python cpplint.py --recursive src 2>&1 | tee ./checker_output/cpplint.txt'
                sh 'cppcheck --enable=all --inconclusive --xml --xml-version=2 -i"TIRTOS Build" -itest/ . 2> ./checker_output/cppcheck.xml'
                // This is the ID of the credential to use, not the credential itself
                withCredentials([string(credentialsId: '98993260-e306-44c2-b523-a70fe6746f6c', variable: 'personal_access_token')]) {
                    step([
                        $class: 'ViolationsToGitLabRecorder',
                        config: [
                            gitLabUrl: 'https://git.spaceprogram.melbourne',
                            projectId: env.gitlabMergeRequestTargetProjectId,
                            mergeRequestId: env.gitlabMergeRequestIid,
                            commentOnlyChangedContent: true,
                            createCommentWithAllSingleFileComments: true,
                            minSeverity: 'INFO',
                            useApiToken: true,
                            apiToken: "${personal_access_token}",
                            useApiTokenCredentials: false,
                            apiTokenCredentialsId: "",
                            apiTokenPrivate: true,
                            authMethodHeader: true,
                            ignoreCertificateErrors: true,
                            keepOldComments: false,
                            shouldSetWip: true,
                            violationConfigs: [
                                [ pattern: '.*/checker_output/.*\\.xml$', parser: 'CPPCHECK', reporter: 'CPPCHECK' ]
                            ]
                        ]
                    ])
                }
            }
        }
    }
}
